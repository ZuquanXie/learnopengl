#version 330 core

in vec3 BColor;
in vec2 TexCoord;

out vec4 Frag_Color;

uniform sampler2D aTexture;

void main(void)
{
	Frag_Color = texture(aTexture, TexCoord);
}