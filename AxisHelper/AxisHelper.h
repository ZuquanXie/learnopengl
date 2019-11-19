#ifndef AXIS_HELPER_H
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#define AXIS_HELPER_H

namespace AxisHelper {
    const GLchar* vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec3 aColor;\n"
        "out vec3 TheColor;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 projectionMatrix;\n"
        "void main(void)\n"
        "{\n"
        "   gl_Position = projectionMatrix * viewMatrix * vec4(aPos, 1.0f);\n"
        "   TheColor = aColor;\n"
        "}\n\0";

    const GLchar* fragmentShaderSource =
        "#version 330 core\n"
        "in vec3 TheColor;\n"
        "out vec4 Frag_Color;\n"
        "void main(void)\n"
        "{\n"
        "   Frag_Color = vec4(TheColor, 1.0f);\n"
        "}\n\0";

    class AxisHelper
    {
    public:
        GLuint VAO;
        GLuint VBO;
        GLuint ID;

        AxisHelper()
        {
            VAO = NULL;
            VBO = NULL;
            ID = NULL;
        }

        AxisHelper(float weight, float xLength, float yLength, float zLength)
        {
            // init shader program
            GLuint vertexShader, fragmentShader;
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);
            checkCompile(vertexShader, "SHADER");
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);
            checkCompile(fragmentShader, "SHADER");
            ID = glCreateProgram();
            glAttachShader(ID, vertexShader);
            glAttachShader(ID, fragmentShader);
            glLinkProgram(ID);
            checkCompile(ID, "PROGRAM");
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glUseProgram(ID);

            // init buffers
            float vertices[36];
            createVertices(xLength, yLength, zLength, vertices);
            // VAO, VBO
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void draw(GLfloat* viewMatrix, GLfloat* projectionMatrix)
        {
            glUseProgram(ID);
            glUniformMatrix4fv(
                glGetUniformLocation(ID, "viewMatrix"),
                1,
                GL_FALSE,
                viewMatrix
            );
            glUniformMatrix4fv(
                glGetUniformLocation(ID, "projectionMatrix"),
                1,
                GL_FALSE,
                projectionMatrix
            );
            glBindVertexArray(VAO);
            glDrawArrays(GL_LINES, 0, 6);
        }

        void remove()
        {
            glDeleteBuffers(1, &VBO);
            glDeleteVertexArrays(1, &VAO);
            glDeleteProgram(ID);
        }

    private:
        void createVertices(float xLength, float yLength, float zLength, float* vptr)
        {
            float half[3] = { xLength / 2, yLength / 2, zLength / 2 };
            int i, j, k;
            i = 0;
            while (i < 3)
            {
                k = 0;
                while (k < 2)
                {
                    j = 0;
                    while (j < 6)
                    {
                        if (j == i)
                            *vptr++ = half[i];
                        else if (j == i + 3)
                            *vptr++ = 1.0f;
                        else
                            *vptr++ = 0.0f;
                        j++;
                    }
                    half[i] = -half[i];
                    k++;
                }
                i++;
            }
        }

        void checkCompile(unsigned int target, std::string type)
        {
            const int INFO_LOG_LENGTH = 512;
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
}
#endif