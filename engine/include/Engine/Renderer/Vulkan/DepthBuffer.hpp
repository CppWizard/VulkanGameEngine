#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>
#include <Engine/Renderer/Vulkan/Swapchain.hpp>

#include <vulkan/vulkan.h>

namespace Engine
{
    class DepthBuffer
    {
    public:
        DepthBuffer(VulkanContext& context, const Swapchain& swapchain);
        ~DepthBuffer();

        DepthBuffer(const DepthBuffer&) = delete;
        DepthBuffer& operator=(const DepthBuffer&) = delete;

        void Recreate(const Swapchain& swapchain);

        VkImageView GetImageView() const { return m_ImageView; }
        VkFormat GetFormat() const { return m_Format; }

    private:
        void Create(const Swapchain& swapchain);
        void Cleanup();

        VkFormat FindDepthFormat() const;
        VkFormat FindSupportedFormat(
            const std::vector<VkFormat>& candidates,
            VkImageTiling tiling,
            VkFormatFeatureFlags features
        ) const;

        uint32_t FindMemoryType(
            uint32_t typeFilter,
            VkMemoryPropertyFlags properties
        ) const;

    private:
        VulkanContext& m_Context;

        VkImage m_Image = VK_NULL_HANDLE;
        VkDeviceMemory m_Memory = VK_NULL_HANDLE;
        VkImageView m_ImageView = VK_NULL_HANDLE;

        VkFormat m_Format = VK_FORMAT_UNDEFINED;
    };
}