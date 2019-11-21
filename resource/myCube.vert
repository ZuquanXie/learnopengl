#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform vec3 lightPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 viewForNormal;

out vec3 Normal;
out vec3 FragPosition;
out vec3 LightPosition;

void main(void)
{
	vec4 vPoint = view * model * vec4(aPos, 1.0f);
	gl_Position = projection * vPoint;
	Normal = normalize(viewForNormal * aNormal);
	FragPosition = vec3(vPoint);
	LightPosition = vec3(view * model * vec4(lightPosition, 1.0f));
}
