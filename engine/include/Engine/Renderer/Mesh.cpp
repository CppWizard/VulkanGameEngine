#include <Engine/Renderer/Mesh.hpp>

namespace Engine
{
    Mesh::Mesh(
        VulkanContext& context,
        const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices
    )
    {
        m_VertexBuffer =
            std::make_unique<VertexBuffer>(context, vertices);

        m_IndexBuffer =
            std::make_unique<IndexBuffer>(context, indices);
    }

    const VertexBuffer& Mesh::GetVertexBuffer() const
    {
        return *m_VertexBuffer;
    }

    const IndexBuffer& Mesh::GetIndexBuffer() const
    {
        return *m_IndexBuffer;
    }

    uint32_t Mesh::GetIndexCount() const
    {
        return m_IndexBuffer->GetIndexCount();
    }
}