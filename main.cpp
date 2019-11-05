#include <iostream>
#include <ContextProvider.h>
#include <shader.h>
#include <stb/stb_image.h>

void init(GLFWwindow* w);
void draw(GLFWwindow* w);
void remove(GLFWwindow* w);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLuint programId;
GLuint VAO, VBO, EBO;
GLuint tex1, tex2;
GLfloat mixRatio = 0.3f;

int main()
{
	int result;

	setScreenSize(SCR_WIDTH, SCR_HEIGHT);
	result = contextProvider(init, draw, remove);

	return result;
}

void init(GLFWwindow* window)
{
    Shader shaderProgram("./resource/vertex.vs", "./resource/fragment.fs");
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
    glGenTextures(1, &tex1);
    glBindTexture(GL_TEXTURE_2D, tex1);
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
    glGenTextures(1, &tex2);
    glBindTexture(GL_TEXTURE_2D, tex2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    imgData = stbi_load("./resource/forbidden.png", &width, &height, &nrChannels, 0);
    if (imgData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "LOAD TEXTURE IMAGE FAILED" << std::endl;
    }
    stbi_image_free(imgData);
    shaderProgram.use();
    glUniform1i(glGetUniformLocation(shaderProgram.ID, "aTexture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram.ID, "aTexture2"), 1);
    glUniform1f(glGetUniformLocation(shaderProgram.ID, "mixRatio"), mixRatio);

	// set draw mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void draw(GLFWwindow* window)
{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(programId);

        // 根据上下键设置两个纹理的混合比例
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            mixRatio = 0.8;
            glUniform1f(glGetUniformLocation(programId, "mixRatio"), mixRatio);
        }
        else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            mixRatio = 0.3;
            glUniform1f(glGetUniformLocation(programId, "mixRatio"), mixRatio);
        }

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void remove(GLFWwindow* window)
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
