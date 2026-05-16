#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>
#include <Engine/Renderer/Vulkan/SyncObjects.hpp>

#include <vulkan/vulkan.h>

namespace Engine
{
    class DescriptorPool
    {
    public:
        explicit DescriptorPool(VulkanContext& context);
        ~DescriptorPool();

        VkDescriptorPool GetHandle() const { return m_Pool; }

    private:
        VulkanContext& m_Context;
        VkDescriptorPool m_Pool = VK_NULL_HANDLE;
    };
}