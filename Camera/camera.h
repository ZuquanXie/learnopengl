#ifndef CAMERA_H
#define CAMERA_H
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float PITCH = 0.0f;
const float YAW = 0.0f;

const float MOVEMENT_SPEED = 3.0f;
const float SENSITIVITY = 0.01f;
const float ZOOM = 45.0f;

enum class Camera_Movement {
    FORWARD, BACKWARD, RIGHT, LEFT
};

class Camera
{
public:
    // camera
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Right;
    glm::vec3 Up;
    glm::vec3 WorldUp;

    // Euler angles
    float Pitch;
    float Yaw;

    // camera options
    float MovementSpeed;
    float Sensitivity;
    float Zoom;

    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float pitch = PITCH,
        float yaw = YAW
    ): Front(glm::vec3(0.0f, 0.0f, 1.0f)), MovementSpeed(MOVEMENT_SPEED), Sensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Pitch = pitch;
        Yaw = yaw;
        updateVector();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void ProcessMove(Camera_Movement direction, float deltaTime)
    {
        if (direction == Camera_Movement::FORWARD)
        {
            Position += Front * deltaTime * MovementSpeed;
        }
        if (direction == Camera_Movement::BACKWARD)
        {
            Position -= Front * deltaTime * MovementSpeed;
        }
        if (direction == Camera_Movement::RIGHT)
        {
            Position += Right * deltaTime * MovementSpeed;
        }
        if (direction == Camera_Movement::LEFT)
        {
            Position -= Right * deltaTime * MovementSpeed;
        }
    }

    void ProcessMouseMove(float dx, float dy)
    {
        Pitch -= dy * Sensitivity;
        Yaw -= dx * Sensitivity;

        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        updateVector();
    }

    void ProcessScroll(float dx, float dy)
    {
        Zoom += dy;

        if (Zoom > 45.0f)
            Zoom = 45.0f;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
    }

private:
    void updateVector()
    {
        glm::vec3 front;
        front.x = cos(Pitch) * sin(Yaw);
        front.y = sin(Pitch);
        front.z = cos(Pitch) * cos(Yaw);
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif