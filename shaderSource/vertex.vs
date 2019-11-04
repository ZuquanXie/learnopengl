#version 330 core

layout(location = 0) in vec3 aPos;
out vec4 aColor;

void main(void)
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	aColor = vec4(aPos.x, aPos.y, aPos.z, 0.5f);
}