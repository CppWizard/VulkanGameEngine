#include <Engine/Renderer/Vulkan/VertexBuffer.hpp>

namespace Engine
{
    VertexBuffer::VertexBuffer(
        VulkanContext& context,
        const std::vector<Vertex>& vertices
    )
    {
        m_VertexCount = static_cast<uint32_t>(vertices.size());

        VkDeviceSize bufferSize =
            sizeof(Vertex) * vertices.size();

        m_Buffer = std::make_unique<Buffer>(
            context,
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        m_Buffer->SetData(vertices.data(), bufferSize);
    }
}