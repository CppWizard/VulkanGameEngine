#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>

#include <vulkan/vulkan.h>

namespace Engine
{
    class Buffer
    {
    public:
        Buffer(
            VulkanContext& context,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties
        );

        ~Buffer();

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        VkBuffer GetHandle() const { return m_Buffer; }
        VkDeviceMemory GetMemory() const { return m_Memory; }
        VkDeviceSize GetSize() const { return m_Size; }

        void SetData(const void* data, VkDeviceSize size);

    private:
        void Create(
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties
        );

        void Cleanup();

        uint32_t FindMemoryType(
            uint32_t typeFilter,
            VkMemoryPropertyFlags properties
        ) const;

    private:
        VulkanContext& m_Context;

        VkBuffer m_Buffer = VK_NULL_HANDLE;
        VkDeviceMemory m_Memory = VK_NULL_HANDLE;
        VkDeviceSize m_Size = 0;
    };
}