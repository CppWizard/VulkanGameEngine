#include <Engine/Renderer/Vulkan/DepthBuffer.hpp>

#include <stdexcept>
#include <vector>

namespace Engine
{
    DepthBuffer::DepthBuffer(
        VulkanContext& context,
        const Swapchain& swapchain
    )
        : m_Context(context)
    {
        Create(swapchain);
    }

    DepthBuffer::~DepthBuffer()
    {
        Cleanup();
    }

    void DepthBuffer::Recreate(const Swapchain& swapchain)
    {
        Cleanup();
        Create(swapchain);
    }

    void DepthBuffer::Create(const Swapchain& swapchain)
    {
        m_Format = FindDepthFormat();

        VkExtent2D extent = swapchain.GetExtent();

        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = extent.width;
        imageInfo.extent.height = extent.height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = m_Format;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(
            m_Context.getDevice(),
            &imageInfo,
            nullptr,
            &m_Image) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create depth image.");
        }

        VkMemoryRequirements memoryRequirements{};
        vkGetImageMemoryRequirements(
            m_Context.getDevice(),
            m_Image,
            &memoryRequirements
        );

        VkMemoryAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = FindMemoryType(
            memoryRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        if (vkAllocateMemory(
            m_Context.getDevice(),
            &allocateInfo,
            nullptr,
            &m_Memory) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate depth image memory.");
        }

        vkBindImageMemory(
            m_Context.getDevice(),
            m_Image,
            m_Memory,
            0
        );

        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_Image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = m_Format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(
            m_Context.getDevice(),
            &viewInfo,
            nullptr,
            &m_ImageView) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create depth image view.");
        }
    }

    void DepthBuffer::Cleanup()
    {
        VkDevice device = m_Context.getDevice();

        if (m_ImageView)
        {
            vkDestroyImageView(device, m_ImageView, nullptr);
            m_ImageView = VK_NULL_HANDLE;
        }

        if (m_Image)
        {
            vkDestroyImage(device, m_Image, nullptr);
            m_Image = VK_NULL_HANDLE;
        }

        if (m_Memory)
        {
            vkFreeMemory(device, m_Memory, nullptr);
            m_Memory = VK_NULL_HANDLE;
        }
    }

    VkFormat DepthBuffer::FindDepthFormat() const
    {
        return FindSupportedFormat(
            {
                VK_FORMAT_D32_SFLOAT,
                VK_FORMAT_D32_SFLOAT_S8_UINT,
                VK_FORMAT_D24_UNORM_S8_UINT
            },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    VkFormat DepthBuffer::FindSupportedFormat(
        const std::vector<VkFormat>& candidates,
        VkImageTiling tiling,
        VkFormatFeatureFlags features
    ) const
    {
        for (VkFormat format : candidates)
        {
            VkFormatProperties properties{};
            vkGetPhysicalDeviceFormatProperties(
                m_Context.getPhysicalDevice(),
                format,
                &properties
            );

            if (tiling == VK_IMAGE_TILING_LINEAR &&
                (properties.linearTilingFeatures & features) == features)
            {
                return format;
            }

            if (tiling == VK_IMAGE_TILING_OPTIMAL &&
                (properties.optimalTilingFeatures & features) == features)
            {
                return format;
            }
        }

        throw std::runtime_error("Failed to find supported depth format.");
    }

    uint32_t DepthBuffer::FindMemoryType(
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties
    ) const
    {
        VkPhysicalDeviceMemoryProperties memoryProperties{};
        vkGetPhysicalDeviceMemoryProperties(
            m_Context.getPhysicalDevice(),
            &memoryProperties
        );

        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) &&
                (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        throw std::runtime_error("Failed to find suitable depth memory type.");
    }
}