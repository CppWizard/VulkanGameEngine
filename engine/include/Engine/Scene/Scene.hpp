#pragma once

#include <Engine/Scene/SceneObject.hpp>

#include <memory>
#include <vector>

namespace Engine
{
    class Scene
    {
    public:
        SceneObject& CreateObject(
            const std::string& name,
            std::shared_ptr<Mesh> mesh
        )
        {
            m_Objects.push_back(
                std::make_unique<SceneObject>(name, std::move(mesh))
            );

            return *m_Objects.back();
        }

        const std::vector<std::unique_ptr<SceneObject>>& GetObjects() const
        {
            return m_Objects;
        }

        std::vector<std::unique_ptr<SceneObject>>& GetObjects()
        {
            return m_Objects;
        }

    private:
        std::vector<std::unique_ptr<SceneObject>> m_Objects;
    };
}