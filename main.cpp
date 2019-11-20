#include <iostream>
#include <ContextProvider.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <camera.h>
#include <shader.h>
#include <AxisHelper.h>

void printFloatVec(unsigned int d, float* vec);
void init(GLFWwindow* w);
void draw(GLFWwindow* w);
void remove(GLFWwindow* w);
void processKeyAction(GLFWwindow* w);
void mouseInputCallback(GLFWwindow* w, double x, double y);
void mouseScrollCallback(GLFWwindow* w, double x, double y);
void createCubeBuffers(GLuint &VBO, GLuint &EBO, float size);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera;
float deltaTime;
float lastFrame = (float)glfwGetTime();
bool mouseFirst = true;
double lastMX = (double)SCR_WIDTH / 2;
double lastMY = (double)SCR_HEIGHT / 2;

AxisHelper::AxisHelper axisHelper;

// buffers
GLuint cubeVBO, cubeEBO;

// lamp
GLuint lampVAO;
glm::mat4 lampModel;
Shader lamp;

// actor
GLuint actorVAO;
glm::mat4 actorModel;
Shader actor;

int main()
{
	int result;

	setScreenSize(SCR_WIDTH, SCR_HEIGHT);
	result = contextProvider(init, draw, remove);

	return result;
}

void init(GLFWwindow* window)
{
    // camera
    camera = Camera(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // axisHelper
    axisHelper = AxisHelper::AxisHelper(0.004f, 2.0f, 2.0f, 2.0f);
    // buffers
    createCubeBuffers(cubeVBO, cubeEBO, 1.0f);
    // actor
    actor = Shader("./resource/myCube.vert", "./resource/myCube.frag");
    actorModel = glm::mat4(1.0f);
    glGenVertexArrays(1, &actorVAO);
    glBindVertexArray(actorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // lamp
    lamp = Shader("./resource/myLamp.vert", "./resource/myLamp.frag");
    lampModel = glm::mat4(1.0f);
    lampModel = glm::translate(lampModel, glm::vec3(0, 5.0f, 0));
    lampModel = glm::scale(lampModel, glm::vec3(0.3f, 0.3f, 0.3f));
    glGenVertexArrays(1, &lampVAO);
    glBindVertexArray(lampVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

        // lamp
        lamp.use();
        lamp.setMat4("model", &lampModel[0][0]);
        lamp.setMat4("view", &viewMatrix[0][0]);
        lamp.setMat4("projection", &projectionMatrix[0][0]);
        glBindVertexArray(lampVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // actor
        actor.use();
        actor.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        actor.setVec3("objectColor", 0.0f, 1.0f, 0.0f);
        actor.setMat4("model", &actorModel[0][0]);
        actor.setMat4("view", &viewMatrix[0][0]);
        actor.setMat4("projection", &projectionMatrix[0][0]);
        glBindVertexArray(actorVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
}

void remove(GLFWwindow* window)
{
    axisHelper.remove();
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

void createCubeBuffers(GLuint &VBO, GLuint &EBO, float size)
{
    float hl = size / 2;
    float vertices[24] = {
        -hl, hl, hl,
        hl, hl, hl,
        hl, -hl, hl,
        -hl, -hl, hl,
        -hl, hl, -hl,
        hl, hl, -hl,
        hl, -hl, -hl,
        -hl, -hl, -hl
    };
    unsigned int indices[36] = {
        // front
        0, 1, 2,
        0, 2, 3,
        // back
        5, 4, 7,
        5, 6, 7,
        // top
        4, 5, 1,
        4, 1, 0,
        // bottom
        2, 3, 6,
        2, 6, 7,
        // left
        4, 0, 3,
        4, 3, 7,
        // right
        1, 5, 6,
        1, 6, 2
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}
