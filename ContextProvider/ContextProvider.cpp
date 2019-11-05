#include "ContextProvider.h"

void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

unsigned int initilized = 0;

unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;

void setScreenSize(unsigned int width, unsigned int height)
{
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	if (initilized == 1)
		glViewport(0, 0, width, height);
}

int contextProvider(hookFn beforeDraw, hookFn draw, hookFn beforeClose)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn opengl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Create window failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// 设置窗口大小发生变化时的回调函数（根据当前窗口大小重新设置视口）
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

	// 检测glad是否成功加载
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Load GLAD failed" << std::endl;
		return -1;
	}
	initilized = 1;

	beforeDraw(window);

	while (!glfwWindowShouldClose(window))
	{
		// 用户输入
		processInput(window);

		draw(window);

		// 交换缓冲
		glfwSwapBuffers(window);
		// 轮询事件
		glfwPollEvents();
	}

	beforeClose(window);

	glfwTerminate();

	return 0;
}

void framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
