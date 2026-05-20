#include <Engine/Renderer/Vulkan/UniformBuffers.hpp>

namespace Engine
{
    UniformBuffers::UniformBuffers(VulkanContext& context)
    {
        m_Buffers.resize(SyncObjects::MaxFramesInFlight);

        for (auto& buffer : m_Buffers)
        {
            buffer = std::make_unique<Buffer>(
                context,
                sizeof(UniformBufferObject),
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            );
        }
    }

    void UniformBuffers::Update(
        uint32_t frameIndex,
        const UniformBufferObject& ubo)
    {
        if (frameIndex >= m_Buffers.size() || !m_Buffers[frameIndex])
        {
            throw std::runtime_error("Invalid uniform buffer frame index.");
        }

        m_Buffers[frameIndex]->SetData(&ubo, sizeof(UniformBufferObject));
    }

    VkBuffer UniformBuffers::GetBuffer(uint32_t frameIndex) const
    {
        return m_Buffers[frameIndex]->GetHandle();
    }

    VkDeviceSize UniformBuffers::GetBufferSize() const
    {
        return sizeof(UniformBufferObject);
    }
}