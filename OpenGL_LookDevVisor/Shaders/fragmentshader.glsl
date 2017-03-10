#version 330 core

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
	vec3  aim;
	
	int   type; // 0= Directional, 1=Point, 2=Spot
};



//Inputs
uniform Material mat;
uniform Light    light;
uniform vec3 cameraPosition;
//

//Shader Global Variables
vec3 ambient;
vec3 norm; 
vec3 lightDir;
vec3 diffuseContribution;
vec3 specularContribution;

float attenuation()
{
  	float distance    = length(light.position - FragPos);
  	float attenuation = 1.0f/ (light.constant+light.linear*distance + light.quadratic*(distance*distance));
  	return attenuation; 
};

void lightingCalculation()
{
	ambient 	= light.ambient*vec3(texture(mat.diffuse,TexCoord));
	norm		= normalize(Normal);   
	
	//Diffuse contribution
	float diffIntensity = max(dot(norm,lightDir),0.0f); 
	diffuseContribution = light.color*diffIntensity*vec3(texture(mat.diffuse,TexCoord)); 
	
	//Specular contribution
	vec3 viewDir    = normalize(cameraPosition-FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float specAmount= pow(max(dot(viewDir,reflectDir),0.0),mat.shininess);
	specularContribution = light.specular*(specAmount*vec3(texture(mat.specular,TexCoord)));
}

void main()
{
	lightDir    = normalize(light.position-FragPos);
	vec4 result;
	
	if (light.type == 1)
	{	
		lightingCalculation();
		float attenuation = attenuation();
		ambient *= attenuation;
		diffuseContribution *= attenuation;
		specularContribution *= attenuation;
		vec4 result = vec4(ambient+diffuseContribution+specularContribution,1.0f);
	}
	if (light.type == 2)
	{
		float theta = dot(lightDir, normalize(-light.aim));
		if (theta > light.cutoff)
		{
			lightingCalculation();
			result = vec4(ambient+diffuseContribution+specularContribution,1.0f);
		}
		else
		{
			result = vec4(light.ambient*vec3(texture(mat.diffuse,TexCoord)),1.0f);
		}
	}
	
    color = result;
}