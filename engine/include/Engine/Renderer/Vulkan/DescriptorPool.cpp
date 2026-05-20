#include <Engine/Renderer/Vulkan/DescriptorPool.hpp>
#include <stdexcept>
#include <array>

namespace Engine
{
    DescriptorPool::DescriptorPool(VulkanContext& context)
        : m_Context(context)
    {
        std::array<VkDescriptorPoolSize, 2> poolSizes{};

        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = SyncObjects::MaxFramesInFlight;

        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = 512;

        VkDescriptorPoolCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        info.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        info.pPoolSizes = poolSizes.data();
        info.maxSets = SyncObjects::MaxFramesInFlight + 512;

        if (vkCreateDescriptorPool(
            m_Context.getDevice(),
            &info,
            nullptr,
            &m_Pool) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create descriptor pool.");
        }
    }

    DescriptorPool::~DescriptorPool()
    {
        if (m_Pool)
        {
            vkDestroyDescriptorPool(m_Context.getDevice(), m_Pool, nullptr);
        }
    }
}