#version 330 core
struct Material {
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};
struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;

out vec4 Frag_Color;

uniform Material material;
uniform Light light;
uniform mat4 model;
uniform mat4 view;

void main(void)
{
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

	vec3 lightPosition = vec3(view * model * vec4(light.position, 1.0f));
	vec3 lightDirection = normalize(lightPosition - FragPosition);
	float diff = max(0.0f, dot(Normal, lightDirection));
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));

	vec3 reflectDir = reflect(-lightDirection, Normal);
	vec3 viewerDirection = normalize(-FragPosition);
	float spec = pow(max(0.0f, dot(reflectDir, viewerDirection)), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	Frag_Color = vec4(ambient + diffuse + specular, 1.0f);
}