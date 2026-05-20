#pragma once

#include <Engine/Renderer/SubMesh.hpp>
#include <Engine/Renderer/Vertex.hpp>
#include <Engine/Renderer/Vulkan/VulkanContext.hpp>

#include <vector>
#include <memory>

namespace Engine
{
    class Mesh
    {
    public:
        explicit Mesh() = default;

        Mesh(
            VulkanContext& context,
            const std::vector<Vertex>& vertices,
            const std::vector<uint32_t>& indices
        );

        void AddSubMesh(std::unique_ptr<SubMesh> subMesh);

        const std::vector<std::unique_ptr<SubMesh>>& GetSubMeshes() const
        {
            return m_SubMeshes;
        }

        const VertexBuffer& GetVertexBuffer() const;
        const IndexBuffer& GetIndexBuffer() const;
        uint32_t GetIndexCount() const;

    private:
        std::vector<std::unique_ptr<SubMesh>> m_SubMeshes;
    };
}