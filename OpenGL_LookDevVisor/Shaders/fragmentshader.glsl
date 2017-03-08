#version 330 core
in vec2 TexCoord;

in  vec3 FragPos;
in  vec3 Normal;
out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

//Sampling Textures input
uniform sampler2D ourTexture1; //Unused
uniform sampler2D ourTexture2; 

void main()
{
	//Ambient Calculation
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	//Lighting direction
	vec3 norm     = normalize(Normal);
	vec3 lightDir = normalize(lightPosition-FragPos);
	
	//Diffuse Contribution
	float diffIntensity = max(dot(norm,lightDir),0.0f); 
	vec3  diffuseContribution = diffIntensity*lightColor; 
	
	//Specular Contribution
	float specularPower = 0.5f;
	float shininess = 100.0f;
	vec3 viewDir    = normalize(cameraPosition-FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float specAmount= pow(max(dot(viewDir,reflectDir),0.0),shininess);
	vec3 specularContribution = specularPower * specAmount *lightColor;
	
	
	//Final Composition
	vec4 result = vec4(ambient+diffuseContribution+specularContribution,1.0f) * texture(ourTexture2, TexCoord);
    color = result;
}