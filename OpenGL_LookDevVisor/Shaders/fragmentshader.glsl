#version 440 core

#define NR_POINT_LIGHTS 3

in  vec2 TexCoord;
in  vec3 FragPos;
in  vec3 Normal;
out vec4 color;


// Shader Interface
struct Material {
	sampler2D diffuse; //Diffuse map
	sampler2D specular;
	sampler2D emission;
	float 	  shininess;
};

// Interface to be conected with all scene lights
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

//

//Shader Global Variables
vec3 ambient;
vec3 norm; 
vec3 lightDir;
vec3 diffuseContribution;
vec3 specularContribution;
vec3 viewDir;

float attenuation(Light light)
{
  	float distance    = length(light.position - FragPos);
  	float attenuation = 1.0f/ (light.constant+light.linear*distance + light.quadratic*(distance*distance));
  	return attenuation; 
};

void ShadingCalculation(Light light)
{
	lightDir    = normalize(light.position-FragPos);
	ambient 	= light.ambient*vec3(texture(mat.diffuse,TexCoord));
	
	//Diffuse contribution
	float diffIntensity = max(dot(norm,lightDir),0.0f); 
	diffuseContribution = light.color*diffIntensity*vec3(texture(mat.diffuse,TexCoord)); 
	
	//Specular contribution
	
	vec3 reflectDir = reflect(-lightDir,norm);
	float specAmount= pow(max(dot(viewDir,reflectDir),0.0),mat.shininess);
	specularContribution = light.specular*(specAmount*vec3(texture(mat.specular,TexCoord)));
}

vec3 pointLightCalculation(Light light)
{
	ShadingCalculation(light);
	float attenuation = attenuation(light);
	ambient *= attenuation;
	diffuseContribution *= attenuation;
	specularContribution *= attenuation;
	vec3 result = vec3(ambient+diffuseContribution+specularContribution);
	return result;
}

vec3 spotLightCalculation(Light light)
{
	ShadingCalculation(light);
	float theta     = dot(lightDir, normalize(-light.aim));
	float epsi      = light.cutoff - light.outcutoff;
	float intensity = clamp( (theta-light.outcutoff) /epsi,0.0,1.0);
	diffuseContribution *= intensity;
	specularContribution*= intensity;
	vec3 result = vec3(ambient+diffuseContribution+specularContribution);
	return result;
}
void main()
{   
	norm		 = normalize(Normal);   
	viewDir      = normalize(cameraPosition-FragPos);

	vec3 lightsOutput;
	for (int i=0; i<NR_POINT_LIGHTS;i++)
	{
		if (lights[i].type==0)
			ShadingCalculation(lights[i]);
		if (lights[i].type==1)
			lightsOutput+=pointLightCalculation(lights[i]);
		if (lights[i].type==2){
			lightsOutput+=spotLightCalculation(lights[i]);}
	}
	color = vec4(lightsOutput,1.0f);
}