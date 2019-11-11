#include <iostream>
#include <ContextProvider.h>
#include <shader.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void printFloatVec(unsigned int d, float* vec);
void init(GLFWwindow* w);
void draw(GLFWwindow* w);
void remove(GLFWwindow* w);
void processKeyAction(GLFWwindow* w);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLuint programId;
GLuint VAO, VBO, EBO;
GLuint texture;
glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
glm::vec3 cameraPosition;
glm::vec3 cameraLookAt;
glm::vec3 cameraUp;
glm::vec3 cameraFront;

int main()
{
	int result;

	setScreenSize(SCR_WIDTH, SCR_HEIGHT);
	result = contextProvider(init, draw, remove);

	return result;
}

void init(GLFWwindow* window)
{
    Shader shaderProgram("./resource/vertex.vert", "./resource/fragment.frag");
    programId = shaderProgram.ID;

	// Vertex Data
	float vertices[] = {
		-0.5f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // top left
		0.5f, 0.5f, 0.0f,      0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 0.0f, // bottom left
	};
	// Vertex Index
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
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

    // viewMatrix(camera)
    cameraPosition = glm::vec3(0.0f, 2.0f, 3.0f);
    cameraLookAt = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 toCamera = glm::normalize(cameraPosition - cameraLookAt);
    glm::vec3 cameraRight  = glm::normalize(glm::cross(toCamera, glm::vec3(0.0f, 1.0f, 0.0f)));
    cameraUp = glm::cross(cameraRight, toCamera);
    cameraFront = -toCamera;
    viewMatrix = glm::lookAt(cameraPosition, cameraLookAt, cameraUp);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    std::cout << "Position: ";
    printFloatVec(3, &cameraPosition[0]);
    std::cout << "Front: ";
    printFloatVec(3, &cameraFront[0]);
    std::cout << "Up: ";
    printFloatVec(3, &cameraUp[0]);

    // projection matrix
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));


	// set draw mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
}

float deltaTime;
float lastFrame = (float)glfwGetTime();
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


        glUseProgram(programId);

        // view
        viewMatrix = glm::lookAt(cameraPosition, cameraFront + cameraPosition, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(programId, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

		glBindVertexArray(VAO);
        // first picture
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(programId, "modelMatrix"), 1, GL_FALSE, &modelMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // second picture
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(glGetUniformLocation(programId, "modelMatrix"), 1, GL_FALSE, &modelMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void remove(GLFWwindow* window)
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void processKeyAction(GLFWwindow* window)
{
    float speed = 7.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    { 
        cameraPosition += cameraFront * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPosition -= cameraFront * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPosition -= glm::cross(cameraFront, cameraUp) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPosition += glm::cross(cameraFront, cameraUp) * speed;
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
