#version 440 core

#define NR_POINT_LIGHTS 8

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 TangentLightPos[NR_POINT_LIGHTS];
out vec3 TangentViewPos;
out vec3 TangentFragPos;

 // TangentBinormalMatrix, transform every vector from Tangent to world space

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos[NR_POINT_LIGHTS];
uniform vec3 viewPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
    FragPos = vec3(model* vec4(position,1.0f));
	
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 T    = normalize(vec3(model * vec4 (tangent, 0.0)));
	vec3 B    = normalize(vec3(model * vec4 (bitangent,0.0)));
	vec3 N    = normalize(vec3(model * vec4 (normal, 0.0)));
    mat3 TBN  = transpose( mat3(T , B, N) );
	TangentViewPos  = TBN * viewPos;
	TangentFragPos  = TBN * FragPos;
	for (int i = 0; i < NR_POINT_LIGHTS;i++){
		TangentLightPos[i] = TBN * lightPos[i];
 	}
}	