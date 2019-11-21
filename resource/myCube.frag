#version 330 core

in vec3 Normal;
in vec3 FragPosition;
in vec3 LightPosition;

out vec4 Frag_Color;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main(void)
{
	float ambientStrength = 0.2f;
	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDirection = normalize(LightPosition - FragPosition);
	float diff = max(0.0f, dot(Normal, lightDirection));
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.4f;
	vec3 reflectDir = reflect(-lightDirection, Normal);
	vec3 viewerDirection = normalize(-FragPosition);
	float spec = pow(max(0.0f, dot(reflectDir, viewerDirection)), 16);
	vec3 specular = specularStrength * spec * lightColor;

	Frag_Color = vec4((ambient + diffuse + specular) * objectColor, 1.0f);
}