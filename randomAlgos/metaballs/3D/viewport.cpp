#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>



namespace meatBalls 
{
   struct s_viewport_matricies
    {
        glm::mat4 p_view;
        glm::mat4 p_perspective;
        float p_fov;
        float p_aspectRatio;
        float p_nearPlane;
        float p_farPlane;
    };

    glm::vec3 g_cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 g_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float g_yaw = -90.0f;
    float g_pitch = 0.0f;


    struct s_viewport_matricies g_viewport_properties;

    void generateViewMatrix()
    {
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
    }

    void move_forward(float ratio)
    {
        g_cameraPos += ratio * g_cameraFront;
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
    }

    void move_back(float ratio)
    {
        g_cameraPos -= ratio * g_cameraFront;
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
    }

    void move_left(float ratio)
    {
        g_cameraPos += glm::normalize(glm::cross(g_cameraFront, g_cameraUp)) * ratio;
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
    }

    void move_right(float ratio)
    {
        g_cameraPos -= glm::normalize(glm::cross(g_cameraFront, g_cameraUp)) * ratio;
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
    }

    void move_up(float ratio)
    {
        g_cameraPos += ratio * g_cameraUp;
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
    }

    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
        front.y = sin(glm::radians(g_pitch));
        front.z = sin(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));

        g_cameraFront = glm::normalize(front);

    }

    void rotate_left(float ratio)
    {
        g_yaw -= ratio;

        updateCameraVectors();
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
    }

    void rotate_right(float ratio)
    {
        g_yaw += ratio;

        updateCameraVectors();
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
    }


    inline std::tuple<uint8_t, uint8_t, uint8_t> skyColour(std::tuple<uint8_t, uint8_t, uint8_t> value)
        {
        auto [a,b,c] = value;
        float angle = std::abs(c*-1.0);
        angle /= std::sqrt(a*a + b*b + c*c);
        angle /= 180.0f;

        return std::tuple<uint8_t, uint8_t, uint8_t>((uint8_t) angle * 78, (uint8_t) angle * 179, (uint8_t) angle * 252);
        

    }
}

