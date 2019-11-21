#version 330 core

in vec3 Normal;
in vec3 FragPosition;

out vec4 Frag_Color;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewerPosition;

void main(void)
{
	vec3 lightDirection = normalize(lightPosition - FragPosition);
	float ambientStrength = 0.2f;
	vec3 ambient = ambientStrength * lightColor;
	float diff = max(0.0f, dot(Normal, lightDirection));
	vec3 diffuse = diff * lightColor;
	float specularStrength = 0.5f;
	vec3 viewerDirection = normalize(viewerPosition - FragPosition);
	vec3 reflectDir = reflect(-lightPosition, Normal);
	float spec = pow(max(0.0f, dot(reflectDir, viewerDirection)), 16);
	vec3 specular = specularStrength * spec * lightColor;

	Frag_Color = vec4((ambient + diffuse + specular) * objectColor, 1.0f);
}