#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#ifndef CONTEXTPROVIDER_H
#define CONTEXTPROVIDER_H

typedef void (*hookFn)();
void setScreenSize(unsigned int width, unsigned int height);
int contextProvider(hookFn beforeDraw, hookFn draw, hookFn beforeClose);

#endif