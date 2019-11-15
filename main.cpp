#include <iostream>
#include <ContextProvider.h>
#include <shader.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <camera.h>

void printFloatVec(unsigned int d, float* vec);
void init(GLFWwindow* w);
void draw(GLFWwindow* w);
void remove(GLFWwindow* w);
void processKeyAction(GLFWwindow* w);
void mouseInputCallback(GLFWwindow* w, double x, double y);
void mouseScrollCallback(GLFWwindow* w, double x, double y);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Shader shaderProgram;
GLuint VAO, VBO, EBO;
GLuint texture;
glm::mat4 modelMatrix;
glm::mat4 projectionMatrix;

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
    shaderProgram = Shader("./resource/vertex.vert", "./resource/fragment.frag");


	// Vertex Data
    float vertices[] = {
        // position            color                    texture position
        0.0f, 0.0f, 1.0f,     1.0f, 1.0f, 1.0f,     0.0f, 0.0f, //front
        1.0f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,     0.0f, 0.0f,//back left1
        0.3f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,     0.0f, 0.0f,//back left2
        -0.3f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f,     0.0f, 0.0f,//back right2
        -1.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f,     0.0f, 0.0f,//back right1
        0.0f, -0.3f, -1.0f, 0.8f, 0.8f, 0.8f,     0.0f, 0.0f,//back middle
    };
	// Vertex Index
    unsigned int indices[] = {
        0, 1, 2,
        0, 3, 4,
        0, 2, 5,
        0, 3, 5,
    };

	// VAO, VBO, EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 顶点
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    // 颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // 设置纹理
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
    shaderProgram.use();
    glUniform1i(glGetUniformLocation(shaderProgram.ID, "aTexture"), 0);

    // model matrix
    modelMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

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

        // clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUseProgram(shaderProgram.ID);

        // view
        glm::mat4 viewMatrix = camera.GetViewMatrix();
        glUniformMatrix4fv(
            glGetUniformLocation(shaderProgram.ID, "viewMatrix"),
            1,
            GL_FALSE,
            glm::value_ptr(viewMatrix)
        );

        // projection
        projectionMatrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		glBindVertexArray(VAO);
        // first picture
        modelMatrix = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "modelMatrix"), 1, GL_FALSE, &modelMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}

void remove(GLFWwindow* window)
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
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
