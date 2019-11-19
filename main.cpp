#include <iostream>
#include <ContextProvider.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <camera.h>
#include <AxisHelper.h>
#include <Plane.h>

void printFloatVec(unsigned int d, float* vec);
void init(GLFWwindow* w);
void draw(GLFWwindow* w);
void remove(GLFWwindow* w);
void processKeyAction(GLFWwindow* w);
void mouseInputCallback(GLFWwindow* w, double x, double y);
void mouseScrollCallback(GLFWwindow* w, double x, double y);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

AxisHelper::AxisHelper axisHelper;
Plane::Plane plane;
GLuint texture;

Camera camera;
float deltaTime;
float lastFrame = (float)glfwGetTime();
bool mouseFirst = true;
double lastMX = (double)SCR_WIDTH / 2;
double lastMY = (double)SCR_HEIGHT / 2;

int main()
{
	int result;

	setScreenSize(SCR_WIDTH, SCR_HEIGHT);
	result = contextProvider(init, draw, remove);

	return result;
}

void init(GLFWwindow* window)
{
    axisHelper = AxisHelper::AxisHelper(0.004f, 2.0f, 2.0f, 2.0f);
    plane = Plane::Plane(2.0f, 0.3f, 1.0f);

    // ¥¥Ω®Œ∆¿Ì
    int width, height, nrChannels;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* imgData = stbi_load("./resource/huitailang.jpg", &width, &height, &nrChannels, 0);
    if (imgData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "LOAD TEXTURE IMAGE FAILED" << std::endl;
    }
    stbi_image_free(imgData);

    // camera
    camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// set draw mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    // mouse event
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseInputCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
}

void draw(GLFWwindow* window)
{
        // delta time
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // key input
        processKeyAction(window);

        // view matrix
        glm::mat4 viewMatrix = camera.GetViewMatrix();
        // projection matrix
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // axis
        axisHelper.draw(&viewMatrix[0][0], &projectionMatrix[0][0]);

        // plane
        plane.draw(&viewMatrix[0][0], &projectionMatrix[0][0]);
}

void remove(GLFWwindow* window)
{
    axisHelper.remove();
    plane.remove();
}

void processKeyAction(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    { 
        camera.ProcessMove(Camera_Movement::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessMove(Camera_Movement::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessMove(Camera_Movement::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessMove(Camera_Movement::RIGHT, deltaTime);
    }
}

void printFloatVec(unsigned int d, float* vec)
{
    unsigned int i;

    i = 0;
    while (i < d)
    {
        std::cout << *vec << " ";
        vec++;
        i++;
    }
    std::cout << std::endl;
}

void mouseInputCallback(GLFWwindow* window, double x, double y)
{
    if (mouseFirst)
    {
        lastMX = x;
        lastMY = y;
        mouseFirst = false;
    }
    camera.ProcessMouseMove((float)(x - lastMX), (float)(y - lastMY));
    lastMX = x;
    lastMY = y;
}

void mouseScrollCallback(GLFWwindow* window, double x, double y)
{
    camera.ProcessScroll((float)x, (float)y);
}
