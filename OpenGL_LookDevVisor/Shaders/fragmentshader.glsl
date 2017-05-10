#version 440 core

#define NR_POINT_LIGHTS 8

out vec4 color; //Result to fragment color

in  vec2 TexCoord;
in  vec3 FragPos;
in  vec3 TangentLightPos[NR_POINT_LIGHTS];
in  vec3 TangentViewPos;
in  vec3 TangentFragPos;



// Shader Interface
struct Material {
	sampler2D diffuse;   //Diffuse map
	sampler2D specular;  //Spec
	sampler2D emission;  //Emission
	sampler2D normalmap; //normalmap
	float 	  shininess; //Roughness parameter
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
vec3 norm; 
vec3 lightDir[NR_POINT_LIGHTS];//I need calculate the lightDir per light.
vec3 diffuseContribution;
vec3 specularContribution;
vec3 viewDir;
vec3 halfwayDir;




float attenuation(Light light)
{
  	float distance    = length(light.position - FragPos);
  	float attenuation = 1.0f/ (light.constant+light.linear*distance + light.quadratic*(distance*distance));
  	return attenuation; 
};

void ShadingCalculation(Light light,int i)
{
	lightDir[i]    = normalize(TangentLightPos[i]-FragPos);
	halfwayDir  = normalize(lightDir[i] + cameraPosition);
	ambient 	= light.ambient;
	
	//Diffuse contribution
	float diffIntensity = max(dot(norm,lightDir[i]),0.0f); 
	diffuseContribution = light.color*diffIntensity; 
	
	//Specular contribution
	//Blinn
	vec3  reflectDir = reflect(-lightDir[i],norm); 
	float specAmount = pow(max(dot(norm,halfwayDir),0.0),mat.shininess);
	
	specularContribution = light.specular*specAmount*vec3(texture(mat.specular, TexCoord));
}

vec3 pointLightCalculation(Light light,int i)
{
	ShadingCalculation(light,i);
	float attenuation = attenuation(light);
	ambient *= attenuation;
	diffuseContribution *= attenuation;
	specularContribution *= attenuation;
	vec3 result = vec3(ambient+diffuseContribution+specularContribution);
	return result;
}

vec3 spotLightCalculation(Light light,int i)
{
	ShadingCalculation(light,i);
	float theta     = dot(lightDir[i], normalize(-light.aim));
	float epsi      = light.cutoff - light.outcutoff;
	float intensity = clamp( (theta-light.outcutoff) /epsi,0.0,1.0);
	diffuseContribution *= intensity;
	specularContribution*= intensity;
	vec3 result = vec3(ambient+diffuseContribution+specularContribution);
	return result;
}
void main()
{    
    // without normal
	if (normalActive){
		norm = texture(mat.normalmap,TexCoord).rgb;
		norm = normalize(norm * 2.0 - 1.0);
	};
	
	viewDir      = normalize(cameraPosition-FragPos);
	
	vec3 lightsOutput;
	for (int i=0; i<NR_POINT_LIGHTS;i++)
	{
		if (lights[i].type==0)
			ShadingCalculation(lights[i],i);
		if (lights[i].type==1)
			lightsOutput+=pointLightCalculation(lights[i],i);
		if (lights[i].type==2){
			lightsOutput+=spotLightCalculation(lights[i],i);
		}
	}
	
	color = vec4(lightsOutput*vec3(texture(mat.diffuse, TexCoord)),1.0f);

}