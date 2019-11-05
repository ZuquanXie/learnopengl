#version 330 core

in vec3 BColor;
in vec2 TexCoord;

out vec4 Frag_Color;

uniform sampler2D aTexture1;
uniform sampler2D aTexture2;
uniform float mixRatio;

void main(void)
{
	Frag_Color = mix(texture(aTexture1, TexCoord), texture(aTexture2, TexCoord), mixRatio);
}