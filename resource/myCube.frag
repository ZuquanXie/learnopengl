#version 330 core

out vec4 Frag_Color;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main(void)
{
	float ambientStrenth = 0.1f;
	vec3 ambient = ambientStrenth * lightColor;
	vec3 result = ambient * objectColor;

	Frag_Color = vec4(result, 1.0f);
}