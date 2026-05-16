#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>
#include <Engine/Renderer/Vulkan/CommandPool.hpp>

#include <vulkan/vulkan.h>

#include <filesystem>

namespace Engine
{
    class Texture2D
    {
    public:
        Texture2D(
            VulkanContext& context,
            const CommandPool& commandPool,
            const std::filesystem::path& path
        );

        ~Texture2D();

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;

        VkImageView GetImageView() const { return m_ImageView; }
        VkSampler GetSampler() const { return m_Sampler; }

    private:
        void CreateTextureImage(
            const CommandPool& commandPool,
            const std::filesystem::path& path
        );

        void CreateImageView();
        void CreateSampler();

        void CreateImage(
            uint32_t width,
            uint32_t height,
            VkFormat format,
            VkImageTiling tiling,
            VkImageUsageFlags usage,
            VkMemoryPropertyFlags properties,
            VkImage& image,
            VkDeviceMemory& memory
        );

        void TransitionImageLayout(
            const CommandPool& commandPool,
            VkImage image,
            VkFormat format,
            VkImageLayout oldLayout,
            VkImageLayout newLayout
        );

        void CopyBufferToImage(
            const CommandPool& commandPool,
            VkBuffer buffer,
            VkImage image,
            uint32_t width,
            uint32_t height
        );

        uint32_t FindMemoryType(
            uint32_t typeFilter,
            VkMemoryPropertyFlags properties
        ) const;

        void Cleanup();

    private:
        VulkanContext& m_Context;

        VkImage m_Image = VK_NULL_HANDLE;
        VkDeviceMemory m_Memory = VK_NULL_HANDLE;
        VkImageView m_ImageView = VK_NULL_HANDLE;
        VkSampler m_Sampler = VK_NULL_HANDLE;
    };
}