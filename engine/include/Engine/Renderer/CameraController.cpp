#include <Engine/Renderer/CameraController.hpp>
#include <Engine/Core/Input.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	CameraController::CameraController(Camera& camera)
		: m_Camera(camera)
	{
		const glm::vec3 direction =
			glm::normalize(m_Camera.GetTarget() - m_Camera.GetPosition());

		m_Yaw = glm::degrees(atan2(direction.z, direction.x));
		m_Pitch = glm::degrees(asin(direction.y));
	}

    void CameraController::Update(float deltaTime)
    {
        glm::vec3 position = m_Camera.GetPosition();

        if (Input::IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
        {
            m_Yaw += static_cast<float>(Input::GetMouseDeltaX()) *
                m_MouseSensitivity;
            m_Pitch -= static_cast<float>(Input::GetMouseDeltaY()) *
                m_MouseSensitivity;
            m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);
        }

        glm::vec3 forward;
        forward.x =
            cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        forward.y =
            sin(glm::radians(m_Pitch));
        forward.z =
            sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

        forward = glm::normalize(forward);

        glm::vec3 right =
            glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

        glm::vec3 up =
            glm::normalize(glm::cross(right, forward));

        float velocity = m_MoveSpeed * deltaTime;

        if (Input::IsKeyDown(GLFW_KEY_W))
            position += forward * velocity;

        if (Input::IsKeyDown(GLFW_KEY_S))
            position -= forward * velocity;

        if (Input::IsKeyDown(GLFW_KEY_A))
            position -= right * velocity;

        if (Input::IsKeyDown(GLFW_KEY_D))
            position += right * velocity;

        if (Input::IsKeyDown(GLFW_KEY_Q))
            position -= up * velocity;

        if (Input::IsKeyDown(GLFW_KEY_E))
            position += up * velocity;

        m_Camera.SetPosition(position);
        m_Camera.SetTarget(position + forward);
    }
}
