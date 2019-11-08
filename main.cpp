#include <iostream>
#include <ContextProvider.h>
#include <shader.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void init(GLFWwindow* w);
void draw(GLFWwindow* w);
void remove(GLFWwindow* w);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLuint programId;
GLuint VAO, VBO, EBO;
GLuint texture;
glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;

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

    // set matrix
    modelMatrix = glm::mat4(1.0f);
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -4.0f));
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// set draw mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void draw(GLFWwindow* window)
{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(programId);

        // 上下键z轴前后移动
        glm::vec3 p = glm::vec3(1.0f, 1.0f, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, 1.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -1.0f));
        }
        // 左右键Y轴左右旋转
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            viewMatrix = glm::rotate(viewMatrix, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            viewMatrix = glm::rotate(viewMatrix, glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        glUniformMatrix4fv(glGetUniformLocation(programId, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(programId, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(programId, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void remove(GLFWwindow* window)
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
