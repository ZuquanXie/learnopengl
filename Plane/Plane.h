#ifndef PLANE_H
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/detail/type_mat4x4.hpp>
#define PLANE_H

namespace Plane {
    const GLchar* vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec3 aColor;\n"
        "out vec3 TheColor;\n"
        "uniform mat4 modelMatrix;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 projectionMatrix;\n"
        "void main(void)\n"
        "{\n"
        "   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);\n"
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

    class Plane
    {
    public:
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        GLuint ID;

        Plane()
        {
            VAO = NULL;
            VBO = NULL;
            EBO = NULL;
            ID = NULL;
        }

        Plane(float length, float width, float height)
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
            glBindVertexArray(VAO);
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            glUseProgram(ID);
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            glUniformMatrix4fv(
                glGetUniformLocation(ID, "modelMatrix"),
                1,
                GL_FALSE,
                &modelMatrix[0][0]
            );
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
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        }

        void remove()
        {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteProgram(ID);
        }

    private:
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
