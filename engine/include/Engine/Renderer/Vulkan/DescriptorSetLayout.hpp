#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>
#include <vulkan/vulkan.h>

namespace Engine
{
    class DescriptorSetLayout
    {
    public:
        explicit DescriptorSetLayout(VulkanContext& context);
        ~DescriptorSetLayout();

        VkDescriptorSetLayout GetHandle() const { return m_Layout; }

    private:
        VulkanContext& m_Context;
        VkDescriptorSetLayout m_Layout = VK_NULL_HANDLE;
    };
}