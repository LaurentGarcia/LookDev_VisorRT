#version 440 core

#define NR_POINT_LIGHTS 8
const float PI = 3.14159265359;

out vec4 FragColor; //Result to fragment color

in vec2 TexCoord;
in vec3 WorldPos;
in vec3 Normal;

// Shader Interface
struct Material {
	sampler2D diffuse;   //Diffuse map
	sampler2D normalmap; //normalmap
	sampler2D metallic;
	sampler2D roughness; //Roughness parameter
	sampler2D ao;
};

// Interface to be connected with all scene lights
struct Light{
	vec3 position;
	vec3 ambient;
	vec3 color;
	vec3 specular;
	
	float constant;
	float quadratic;
	float linear;
	
	float cutoff;
	float outcutoff;
	vec3  aim;
		
	int   type; // 0= Directional, 1=Point, 2=Spot
};


//Inputs
uniform vec3 cameraPosition;
uniform Material mat;
uniform Light lights[NR_POINT_LIGHTS];
uniform bool normalActive;



//Shader Global Variables
vec3 ambient;
vec3 N; 
vec3 lightDir[NR_POINT_LIGHTS];//I need calculate the lightDir per light.
vec3 V;
vec3 halfwayDir;


// BRDF Functions for specular Terms
	// Normal Distribution, Trowbridge-Reitz GGX

float normalDistributionGGX(vec3 N, vec3 H, float roughness)
{
	float alpha  = roughness * roughness;
	float alpha2 = alpha * alpha; 
	float ndoth  = max(dot(N,H),0.0);
	float ndoth2 = ndoth * ndoth;
	
	float nom    = alpha2;
	float denom  = (ndoth2 * (alpha2 -1.0) + 1.0);
	denom        = PI * denom * denom;
	
	return nom/denom;

}

	// Geometry Distribution Schlick GGX
	
float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r 	= (roughness + 1.0);
	float k     = (r*r)/8.0;
	float nom   = NdotV;
	float denom = NdotV*(1-k)+k;
	
	return nom/denom; 

}
	//Smith's method to take account both view directions
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	
	float NdotV = max(dot(N,V),0.0);
	float NdotL = max(dot(N,L),0.0);
	
	float ggx1  = GeometrySchlickGGX(NdotV,roughness);
	float ggx2  = GeometrySchlickGGX(NdotL,roughness);
	
	return ggx1 * ggx2;
}
	
	// Fresnel Aproximation - Schlick
	// F0 IOR index

vec3 schlickFresnel(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


// Irradiance section


void main(){

	N = normalize(Normal);
	V = normalize(cameraPosition-WorldPos); 	

	// Getting info from Textures
	vec3  albedo     = pow(texture(mat.diffuse, TexCoord).rgb, vec3(2.2));
	float metallic  = texture(mat.metallic, TexCoord).r;
    float roughness = texture(mat.roughness, TexCoord).r;
    float ao        = texture(mat.ao, TexCoord).r;

	//Solve integral in direct lighting as sum of num_lights
	vec3 Lo = vec3(0.0);
	for (int i=0; i< NR_POINT_LIGHTS; i++)
	{
		vec3 L = normalize(lights[i].position - WorldPos);
		vec3 H = normalize(V+L);
		
		float distance    = length(lights[i].position - WorldPos);
		float attenuation = 1.0/ ((distance * distance)*0.1);
		vec3  radiance    = lights[i].color * attenuation;
		
		vec3  F0 = vec3(0.04);
		F0 = mix(F0,albedo,metallic);
		
		vec3  F   = schlickFresnel(max(dot(H, V), 0.0),F0);
		float NDF = normalDistributionGGX(N,H,roughness);
		float G   = GeometrySmith(N,V,L,roughness);
		
		// cook-Torrance BRDF (ks)
		vec3 nominator    = NDF * G * F;
		float denominator = 4 * max(dot(N,V),0.0) * max(dot(N,L),0.0) + 0.001;
		vec3 specular = nominator/denominator; 
		vec3 Ks = F;
		vec3 Kd = vec3(1.0) - Ks;
	
		//Latest term of ecuation
		float NdotL = max(dot(N,L),0.0);
		Lo += (Kd * albedo/PI + specular)* radiance * NdotL; // Integral PBR ecuation
	}
	
	vec3 ambient = vec3(0.03)* albedo * ao;
	vec3 color   = ambient + Lo;
	
	color = color / (color + vec3(1.0));
	color = pow(color,vec3(1.0/2.2));
	 
	FragColor = vec4 (color,1.0);
}
















