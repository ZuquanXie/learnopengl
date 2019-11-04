#version 330 core

in vec4 aColor;
out vec4 Frag_Color;

void main(void)
{
	Frag_Color = aColor;
}