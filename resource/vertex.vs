#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aCoord;

out vec3 BColor;
out vec2 TexCoord;

void main(void)
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	BColor = aColor;
	TexCoord = aCoord;
}