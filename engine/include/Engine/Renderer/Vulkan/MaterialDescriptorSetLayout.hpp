#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>
#include <vulkan/vulkan.h>

namespace Engine
{
    class MaterialDescriptorSetLayout
    {
    public:
        explicit MaterialDescriptorSetLayout(VulkanContext& context);
        ~MaterialDescriptorSetLayout();

        VkDescriptorSetLayout GetHandle() const { return m_Layout; }

    private:
        VulkanContext& m_Context;
        VkDescriptorSetLayout m_Layout = VK_NULL_HANDLE;
    };
}