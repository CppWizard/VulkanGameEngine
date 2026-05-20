#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>
#include <Engine/Renderer/Vulkan/MaterialDescriptorSetLayout.hpp>
#include <Engine/Renderer/Vulkan/Texture2D.hpp>

#include <vulkan/vulkan.h>

namespace Engine
{
    class MaterialDescriptorSet
    {
    public:
        MaterialDescriptorSet(
            VulkanContext& context,
            VkDescriptorPool descriptorPool,
            const MaterialDescriptorSetLayout& layout,
            const Texture2D& texture
        );

        VkDescriptorSet GetHandle() const { return m_DescriptorSet; }

    private:
        VulkanContext& m_Context;
        VkDescriptorSet m_DescriptorSet = VK_NULL_HANDLE;
    };
}