#pragma once

#include <Engine/Scene/TransformComponent.hpp>
#include <Engine/Renderer/Mesh.hpp>

#include <memory>
#include <string>

namespace Engine
{
    class SceneObject
    {
    public:
        SceneObject(
            std::string name,
            std::shared_ptr<Mesh> mesh
        )
            : Name(std::move(name)), MeshData(std::move(mesh))
        {}

        std::string Name;
        TransformComponent Transform;
        std::shared_ptr<Mesh> MeshData;
    };
}