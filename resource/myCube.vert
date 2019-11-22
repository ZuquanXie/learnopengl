#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 viewForNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPosition;

void main(void)
{
	vec4 vPoint = view * model * vec4(aPos, 1.0f);
	gl_Position = projection * vPoint;
	TexCoord = aCoord;
	Normal = normalize(viewForNormal * aNormal);
	FragPosition = vec3(vPoint);
}
