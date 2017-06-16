#version 330 core

in vec2 TexCoords;

out vec4 fragColor;

uniform vec3 lightColor;

void main()
{    
    fragColor = vec4(lightColor,1.0f);
}