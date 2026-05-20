#include <Engine/Renderer/Mesh.hpp>

namespace Engine
{
    Mesh::Mesh(
        VulkanContext& context,
        const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices
    )
    {
        auto subMesh = std::make_unique<SubMesh>();

        subMesh->Vertices =
            std::make_unique<VertexBuffer>(context, vertices);

        subMesh->Indices =
            std::make_unique<IndexBuffer>(context, indices);

        m_SubMeshes.push_back(std::move(subMesh));
    }

    void Mesh::AddSubMesh(std::unique_ptr<SubMesh> subMesh)
    {
        m_SubMeshes.push_back(std::move(subMesh));
    }

    const VertexBuffer& Mesh::GetVertexBuffer() const
    {
        return *m_SubMeshes[0]->Vertices;
    }

    const IndexBuffer& Mesh::GetIndexBuffer() const
    {
        return *m_SubMeshes[0]->Indices;
    }

    uint32_t Mesh::GetIndexCount() const
    {
        return m_SubMeshes[0]->GetIndexCount();
    }
}