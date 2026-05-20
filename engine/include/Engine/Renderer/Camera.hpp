#pragma once

#include <glm/glm.hpp>

namespace Engine
{
    class Camera
    {
    public:
        Camera(
            float fov,
            float aspectRatio,
            float nearClip,
            float farClip
        );

        void SetPosition(const glm::vec3& position);
        void SetTarget(const glm::vec3& target);
        void SetAspectRatio(float aspectRatio);

        const glm::vec3& GetPosition() const { return m_Position; }
        const glm::vec3& GetTarget() const { return m_Target; }

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;

    private:
        glm::vec3 m_Position = { 0.0f, 0.0f, 3.0f };
        glm::vec3 m_Target = { 0.0f, 0.0f, 0.0f };

        float m_FOV = 45.0f;
        float m_AspectRatio = 1.0f;
        float m_NearClip = 0.1f;
        float m_FarClip = 100.0f;
    };
}