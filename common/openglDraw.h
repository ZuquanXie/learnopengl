#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#ifndef OPENGL_DRAW_H
#define OPENGL_DRAW_H

typedef void (*hookFn)();
void setScreenSize(unsigned int width, unsigned int height);
int openglDraw(hookFn beforeDraw, hookFn draw, hookFn beforeClose);

#endif