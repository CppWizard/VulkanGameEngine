#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>
#include <Engine/Renderer/Vulkan/DescriptorPool.hpp>
#include <Engine/Renderer/Vulkan/DescriptorSetLayout.hpp>
#include <Engine/Renderer/Vulkan/UniformBuffers.hpp>
#include <Engine/Renderer/Vulkan/SyncObjects.hpp>
#include <Engine/Renderer/Vulkan/Texture2D.hpp>

#include <vector>

namespace Engine
{
    class DescriptorSets
    {
    public:
        DescriptorSets(
            VulkanContext& context,
            const DescriptorPool& pool,
            const DescriptorSetLayout& layout,
            const UniformBuffers& uniformBuffers
        );

        VkDescriptorSet Get(uint32_t frameIndex) const
        {
            return m_DescriptorSets[frameIndex];
        }

    private:
        VulkanContext& m_Context;
        std::vector<VkDescriptorSet> m_DescriptorSets;
    };
}