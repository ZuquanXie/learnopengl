#include <iostream>
#include "openglDraw.h"
#include "helloTriangle-exercise.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	int result;

	setScreenSize(SCR_WIDTH, SCR_HEIGHT);
	result = openglDraw(drawingInit, draw, remove);

	return result;
}
