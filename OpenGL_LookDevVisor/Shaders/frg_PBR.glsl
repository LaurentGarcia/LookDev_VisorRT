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
	float     F0;
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

uniform samplerCube irradianceMap; // Precalculated HDR as Cubemap Irradiance.
uniform samplerCube prefilterMap;
uniform sampler2D   brdfLUT;


//SHADER GLOBAL VARIABLES
vec3 ambient;
vec3 N; 
vec3 lightDir[NR_POINT_LIGHTS];//I need calculate the lightDir per light.
vec3 V;
vec3 halfwayDir;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(mat.normalmap, TexCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoord);
    vec2 st2 = dFdy(TexCoord);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   
// ----------------------------------------------------------------------------


float DirectionalightAttenuation(Light light)
{
	return 1.0; // no attenuation for directional light
};

float pointLightAttenuation(Light light)
{
	float  distance    = length(light.position - WorldPos);
  	float  attenuation = 1.0f/ (light.constant+light.linear*distance + light.quadratic*(distance*distance));
  	return attenuation;

};

float SpotLightAttenuation(Light light,int i)
{
	float theta     = dot(lightDir[i], normalize(-light.aim));
	float epsi      = light.cutoff - light.outcutoff;
	float intensity = clamp( (theta-light.outcutoff) /epsi,0.0,1.0);
	return intensity;

};

void main()
{		
	
    // material properties
    vec3 albedo = pow(texture(mat.diffuse, TexCoord).rgb, vec3(2.2));
    float metallic = texture(mat.metallic, TexCoord).r;
    float roughness = texture(mat.roughness, TexCoord).r;
    float ao = texture(mat.ao, TexCoord).r;
       
    // input lighting data
    vec3 N = getNormalFromMap();
    vec3 V = normalize(cameraPosition - WorldPos);
    vec3 R = reflect(-V, N); 

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = mix(vec3(mat.F0), albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < NR_POINT_LIGHTS; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(lights[i].position - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lights[i].position - WorldPos);
       
        //Per light attenuation Calculation
        //float attenuation = 1.0 / (distance * distance);
       	
       	float attenuation;
       	
       	if (lights[i].type == 0)
       		attenuation = DirectionalightAttenuation(lights[i]);
       	if (lights[i].type == 1)
       		attenuation = pointLightAttenuation(lights[i]);
       	if (lights[i].type == 2)
       		attenuation = SpotLightAttenuation(lights[i],i);
       
       
        vec3 radiance = lights[i].color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
        vec3 nominator    = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = (nominator / denominator) * attenuation * lights[i].specular;
        
         // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	                
            
        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse      = irradiance * albedo;
    
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color , 1.0);
}










