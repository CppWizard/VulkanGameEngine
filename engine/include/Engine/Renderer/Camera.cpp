#include <Engine/Renderer/Camera.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    Camera::Camera(
        float fovDegrees,
        float aspectRatio,
        float nearPlane,
        float farPlane
    )
        : m_Fov(fovDegrees),
        m_AspectRatio(aspectRatio),
        m_NearPlane(nearPlane),
        m_FarPlane(farPlane)
    {}

    void Camera::SetPosition(const glm::vec3& position)
    {
        m_Position = position;
    }

    void Camera::SetTarget(const glm::vec3& target)
    {
        m_Target = target;
    }

    const glm::vec3& Camera::GetPosition() const
    {
        return m_Position;
    }

    const glm::vec3& Camera::GetTarget() const
    {
        return m_Target;
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return glm::lookAt(
            m_Position,
            m_Target,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
    }

    glm::mat4 Camera::GetProjectionMatrix() const
    {
        glm::mat4 projection =
            glm::perspective(
                glm::radians(m_Fov),
                m_AspectRatio,
                m_NearPlane,
                m_FarPlane
            );

        // Vulkan clip-space correction
        projection[1][1] *= -1.0f;

        return projection;
    }

    void Camera::SetAspectRatio(float aspectRatio)
    {
        m_AspectRatio = aspectRatio;
    }
}