#include "helloTriangle-exercise.h"

const char* vertexShaderSource =
	"#version 330 core\n"
	"layout(location=0) in vec3 aPos;\n"
	"void main(void)\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\n\0"
;

const char* fragmentShaderSource1 =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main(void)\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0"
;

const char* fragmentShaderSource2 =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main(void)\n"
	"{\n"
	"	FragColor = vec4(0.9f, 0.9f, 0.2f, 1.0f);\n"
	"}\n\0"
;

const float vertices1[] = {
	0.5f, 0.35f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.5f, -0.35f, 0.0f,
	-0.5f, 0.35f, 0.0f,
	0.0f, 0.0f, 0.0f,
	-0.5f, -0.35f, 0.0f,
};

const float vertices2[] = {
	-0.35f, 0.5f, 0.0f,
	0.35f, 0.5f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.35f, -0.5f, 0.0f,
	-0.35f, -0.5f, 0.0f,
};

GLuint vertexShader, fragmentShader, shaderProgram1, shaderProgram2;
GLuint VAO1, VAO2, VBO1, VBO2;

void drawingInit()
{
	int success;
	char infolog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// first shaderProgram
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		std::cout << "[FRAGMENT_COMPILE_FAILED]\n" << infolog << std::endl;
	}
	shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader);
	glLinkProgram(shaderProgram1);
	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram1, 512, NULL, infolog);
		std::cout << "[PROGRAM_LINK_FAILED]\n" << infolog << std::endl;
	}

	// second shaderProgram
	glShaderSource(fragmentShader, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		std::cout << "[FRAGMENT_COMPILE_FAILED]\n" << infolog << std::endl;
	}
	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader);
	glLinkProgram(shaderProgram2);
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infolog);
		std::cout << "[PROGRAM_LINK_FAILED]\n" << infolog << std::endl;
	}

	glGenVertexArrays(1, &VAO1);
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &VBO2);

	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void draw()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram1);
	glBindVertexArray(VAO1);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glUseProgram(shaderProgram2);
	glBindVertexArray(VAO2);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void remove()
{
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VBO2);
}