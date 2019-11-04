#include <iostream>
#include <ContextProvider.h>
#include <shader.h>

void init();
void draw();
void remove();

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 顶点着色器源码
const char* vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main(void)\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\n\0";

// 片段着色器源码
const char* fragmentShaderSource =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main(void)\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0"
;

GLuint programId;
GLuint VAO, VBO, EBO;

int main()
{
	int result;

	setScreenSize(SCR_WIDTH, SCR_HEIGHT);
	result = contextProvider(init, draw, remove);

	return result;
}


void init()
{
	int success;
	char infoLog[512];

    Shader shaderProgram("./shaderSource/vertex.vs", "./shaderSource/fragment.fs");
    programId = shaderProgram.ID;

	// Vertex Data
	float vertices[] = {
		-0.5f, 0.5f, 0.0f, //top left
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f // bottom left
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

	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// set draw mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void draw()
{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programId);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void remove()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
