#include <Engine/Renderer/Vulkan/IndexBuffer.hpp>

namespace Engine
{
    IndexBuffer::IndexBuffer(
        VulkanContext& context,
        const std::vector<uint32_t>& indices
    )
    {
        m_IndexCount = static_cast<uint32_t>(indices.size());

        VkDeviceSize bufferSize =
            sizeof(uint32_t) * indices.size();

        m_Buffer = std::make_unique<Buffer>(
            context,
            bufferSize,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        m_Buffer->SetData(indices.data(), bufferSize);
    }
}