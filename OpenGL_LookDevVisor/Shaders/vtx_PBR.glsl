#version 440 core

#define NR_POINT_LIGHTS 8

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec2 TexCoord;
out vec3 WorldPos;
out vec3 Normal;

 // TangentBinormalMatrix, transform every vector from Tangent to world space

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos[NR_POINT_LIGHTS];
uniform vec3 viewPos;

void main()
{
	TexCoord = texCoord;
	WorldPos = vec3(model  * vec4(position,1.0f));
	Normal   = mat3(model) * normal;
	
	gl_Position = projection * view * vec4(WorldPos,1.0);
}	