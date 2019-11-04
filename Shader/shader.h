#ifndef SHADER_S_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#define SHADER_S_H
#define INFO_LOG_LENGTH 512

class Shader {
public:
	unsigned int ID;

	Shader(GLchar* vShaderPath, GLchar* fShaderPath)
	{
		std::ifstream vFile, fFile;
		std::stringstream vStream, fStream;
		std::string vString, fString;
		const char* vCode;
		const char* fCode;
		GLint success;
		GLchar* infolog;

		// read shader source
		vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			vFile.open(vShaderPath);
			fFile.open(fShaderPath);
			vStream << vFile.rdbuf();
			fStream << fFile.rdbuf();
			vFile.close();
			fFile.close();
			vString = vStream.str();
			fString = fStream.str();
		}
		catch(std::ifstream::failure e)
		{
			std::cout << "SHADER::READ_SOURCE_FILE_FAILED" << std::endl;
		}

		unsigned int vertexShader, fragmentShader;
		// create vertex shader
		vCode = vString.c_str();
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vCode, NULL);
		glCompileShader(vertexShader);
		checkCompile(vertexShader, "SHADER");

		// create fragment shader
		fCode = fString.c_str();
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fCode, NULL);
		glCompileShader(fragmentShader);
		checkCompile(fragmentShader, "SHADER");

		// create shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		checkCompile(ID, "PROGRAM");
	}

	void use()
	{
		glUseProgram(ID);
	}

private:
	void checkCompile(unsigned int target, std::string type)
	{
		char infolog[INFO_LOG_LENGTH];
		GLint success;

		if (type == "SHADER")
		{
			glGetShaderiv(target, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(target, INFO_LOG_LENGTH, NULL, infolog);
				std::cout << "SHADER::COMPILE_FAILED" << infolog << std::endl;
			}
		}
		else if (type == "PROGRAM")
		{
			glGetProgramiv(target, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(target, INFO_LOG_LENGTH, NULL, infolog);
				std::cout << "PROGRAM::LINK_FAILED" << infolog << std::endl;
			}
		}

	}
};

#endif