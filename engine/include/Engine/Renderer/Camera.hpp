#pragma once

#include <glm/glm.hpp>

namespace Engine
{
    class Camera
    {
    public:
        Camera(
            float fovDegrees,
            float aspectRatio,
            float nearPlane,
            float farPlane
        );

        void SetPosition(const glm::vec3& position);
        void SetTarget(const glm::vec3& target);

        const glm::vec3& GetPosition() const;
        const glm::vec3& GetTarget() const;

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;

        void SetAspectRatio(float aspectRatio);

    private:
        glm::vec3 m_Position = { 0.0f, 0.0f, 2.0f };
        glm::vec3 m_Target = { 0.0f, 0.0f, 0.0f };

        float m_Fov;
        float m_AspectRatio;
        float m_NearPlane;
        float m_FarPlane;
    };
}