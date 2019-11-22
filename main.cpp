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
void createCubeVBO(GLuint &VBO, float size);

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
GLuint cubeVBO;

// lamp
GLuint lampVAO;
glm::mat4 lampModel;
glm::vec3 lampPosition;
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
    camera = Camera(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // axisHelper
    axisHelper = AxisHelper::AxisHelper(0.004f, 2.0f, 2.0f, 2.0f);
    // buffers
    createCubeVBO(cubeVBO, 1.0f);
    // lamp
    lamp = Shader("./resource/myLamp.vert", "./resource/myLamp.frag");
    lampPosition = glm::vec3(1.0f, 1.0f, 0.0f);
    lampModel = glm::mat4(1.0f);
    lampModel = glm::translate(lampModel, lampPosition);
    lampModel = glm::scale(lampModel, glm::vec3(0.2f));
    glGenVertexArrays(1, &lampVAO);
    glBindVertexArray(lampVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // actor
    actor = Shader("./resource/myCube.vert", "./resource/myCube.frag");
    actorModel = glm::mat4(1.0f);
    actorModel = glm::rotate(actorModel, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    actor.use();
    // material: white plastic
    actor.setVec3("material.ambient", 0.0f, 0.0f, 0.0f);
    actor.setVec3("material.diffuse", 0.1f, 0.35f, 0.1f);
    actor.setVec3("material.specular", 0.45f, 0.55f, 0.45f);
    actor.setFloat("material.shininess", 0.25f * 128.0f);
    // material: green rubber
    actor.setVec3("material.ambient", 0.0f, 0.05f, 0.0f);
    actor.setVec3("material.diffuse", 0.4f, 0.5f, 0.4f);
    actor.setVec3("material.specular", 0.04f, 0.7f, 0.04f);
    actor.setFloat("material.shininess", 0.078125f * 128.0f);
    // light
    actor.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
    actor.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
    actor.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    glUseProgram(0);
    glGenVertexArrays(1, &actorVAO);
    glBindVertexArray(actorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // axis
        axisHelper.draw(&viewMatrix[0][0], &projectionMatrix[0][0]);

        // lamp
        lampPosition = glm::vec3(-sin(lastFrame), cos(lastFrame), cos(lastFrame));
        lampModel = glm::translate(glm::mat4(1.0f), lampPosition);
        lampModel = glm::scale(lampModel, glm::vec3(0.2f, 0.2f, 0.2f));
        lamp.use();
        lamp.setMat4("model", &lampModel[0][0]);
        lamp.setMat4("view", &viewMatrix[0][0]);
        lamp.setMat4("projection", &projectionMatrix[0][0]);
        glBindVertexArray(lampVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // actor
        glm::mat3 viewForNormal = glm::mat3(glm::transpose(glm::inverse(viewMatrix * actorModel)));
        actor.use();
        actor.setVec3("light.position", &lampPosition[0]);
        actor.setMat4("model", &actorModel[0][0]);
        actor.setMat4("view", &viewMatrix[0][0]);
        actor.setMat4("projection", &projectionMatrix[0][0]);
        actor.setMat3("viewForNormal", &viewForNormal[0][0]);
        glBindVertexArray(actorVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
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

void createCubeVBO(GLuint &VBO,  float size)
{
    float hl = size / 2;
    float vertices[6*6*6] = {
        // back
        -hl, hl, -hl, 0.0f, 0.0f, -1.0f,
        -hl, -hl, -hl, 0.0f, 0.0f, -1.0f,
        hl, -hl, -hl, 0.0f, 0.0f, -1.0f,
        -hl, hl, -hl, 0.0f, 0.0f, -1.0f,
        hl, -hl, -hl, 0.0f, 0.0f, -1.0f,
        hl, hl, -hl, 0.0f, 0.0f, -1.0f,
        // front
        hl, hl, hl, 0.0f, 0.0f, 1.0f,
        hl, -hl, hl, 0.0f, 0.0f, 1.0f,
        -hl, -hl, hl, 0.0f, 0.0f, 1.0f,
        hl, hl, hl, 0.0f, 0.0f, 1.0f,
        -hl, -hl, hl, 0.0f, 0.0f, 1.0f,
        -hl, hl, hl, 0.0f, 0.0f, 1.0f,
        // left
        -hl, hl, hl, -1.0f, 0.0f, 0.0f,
        -hl, -hl, hl, -1.0f, 0.0f, 0.0f,
        -hl, -hl, -hl, -1.0f, 0.0f, 0.0f,
        -hl, hl, hl, -1.0f, 0.0f, 0.0f,
        -hl, -hl, -hl, -1.0f, 0.0f, 0.0f,
        -hl, hl, -hl, -1.0f, 0.0f, 0.0f,
        // right
        hl, hl, -hl, 1.0f, 0.0f, 0.0f,
        hl, -hl, -hl, 1.0f, 0.0f, 0.0f,
        hl, -hl, hl, 1.0f, 0.0f, 0.0f,
        hl, hl, -hl, 1.0f, 0.0f, 0.0f,
        hl, -hl, hl, 1.0f, 0.0f, 0.0f,
        hl, hl, hl, 1.0f, 0.0f, 0.0f,
        // top
        hl, hl, -hl, 0.0f, 1.0f, 0.0f,
        hl, hl, hl, 0.0f, 1.0f, 0.0f,
        -hl, hl, hl, 0.0f, 1.0f, 0.0f,
        hl, hl, -hl, 0.0f, 1.0f, 0.0f,
        -hl, hl, hl, 0.0f, 1.0f, 0.0f,
        -hl, hl, -hl, 0.0f, 1.0f, 0.0f,
        // bottom
        hl, -hl, hl, 0.0f, -1.0f, 0.0f,
        hl, -hl, -hl, 0.0f, -1.0f, 0.0f,
        -hl, -hl, -hl, 0.0f, -1.0f, 0.0f,
        hl, -hl, hl, 0.0f, -1.0f, 0.0f,
        -hl, -hl, -hl, 0.0f, -1.0f, 0.0f,
        -hl, -hl, hl, 0.0f, -1.0f, 0.0f
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}
