#pragma once

#include <Engine/Renderer/Vulkan/VertexBuffer.hpp>
#include <Engine/Renderer/Vulkan/IndexBuffer.hpp>
#include <Engine/Renderer/Material.hpp>

#include <memory>

namespace Engine
{
    struct SubMesh
    {
        std::unique_ptr<VertexBuffer> Vertices;
        std::unique_ptr<IndexBuffer> Indices;
        std::shared_ptr<Material> MaterialData;

        uint32_t GetIndexCount() const
        {
            return Indices ? Indices->GetIndexCount() : 0;
        }
    };
}