#include <Engine/Renderer/Camera.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    Camera::Camera(
        float fov,
        float aspectRatio,
        float nearClip,
        float farClip
    )
        : m_FOV(fov),
        m_AspectRatio(aspectRatio),
        m_NearClip(nearClip),
        m_FarClip(farClip)
    {}

    void Camera::SetPosition(const glm::vec3& position)
    {
        m_Position = position;
    }

    void Camera::SetTarget(const glm::vec3& target)
    {
        m_Target = target;
    }

    void Camera::SetAspectRatio(float aspectRatio)
    {
        m_AspectRatio = aspectRatio;
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
                glm::radians(m_FOV),
                m_AspectRatio,
                m_NearClip,
                m_FarClip
            );

        projection[1][1] *= -1.0f;

        return projection;
    }
}