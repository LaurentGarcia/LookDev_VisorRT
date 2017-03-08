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
};



//
uniform Material mat;
uniform Light    light;
//


uniform vec3 cameraPosition;


//Sampling Textures input
uniform sampler2D ourTexture1; //Unused


void main()
{
	//Ambient Calculation
	
	vec3 ambient = light.ambient*vec3(texture(mat.diffuse,TexCoord));

	//Lighting direction
	
	vec3 norm     = normalize(Normal);
	vec3 lightDir = normalize(light.position-FragPos);
	
	//Diffuse Contribution
	
	float diffIntensity = max(dot(norm,lightDir),0.0f); 
	
	vec3  diffuseContribution = light.color*diffIntensity*vec3(texture(mat.diffuse,TexCoord)); 
	
	//Specular Contribution
	
	vec3 viewDir    = normalize(cameraPosition-FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float specAmount= pow(max(dot(viewDir,reflectDir),0.0),mat.shininess);
	vec3 specularContribution = light.specular*(specAmount*vec3(texture(mat.specular,TexCoord)));
	
	
	//Final Composition
	vec3 emission = vec3(texture(mat.emission,TexCoord));
	vec4 result = vec4(ambient+diffuseContribution+specularContribution+emission,1.0f);
    color = result;
}