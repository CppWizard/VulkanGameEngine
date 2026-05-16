#include <Engine/Renderer/Vulkan/Buffer.hpp>

#include <cstring>
#include <stdexcept>

namespace Engine
{
    Buffer::Buffer(
        VulkanContext& context,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties
    )
        : m_Context(context), m_Size(size)
    {
        Create(size, usage, properties);
    }

    Buffer::~Buffer()
    {
        Cleanup();
    }

    void Buffer::Create(
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties
    )
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(
            m_Context.getDevice(),
            &bufferInfo,
            nullptr,
            &m_Buffer) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create buffer.");
        }

        VkMemoryRequirements memoryRequirements{};
        vkGetBufferMemoryRequirements(
            m_Context.getDevice(),
            m_Buffer,
            &memoryRequirements
        );

        VkMemoryAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex =
            FindMemoryType(memoryRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(
            m_Context.getDevice(),
            &allocateInfo,
            nullptr,
            &m_Memory) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate buffer memory.");
        }

        vkBindBufferMemory(
            m_Context.getDevice(),
            m_Buffer,
            m_Memory,
            0
        );
    }

    void Buffer::Cleanup()
    {
        VkDevice device = m_Context.getDevice();

        if (m_Buffer)
        {
            vkDestroyBuffer(device, m_Buffer, nullptr);
            m_Buffer = VK_NULL_HANDLE;
        }

        if (m_Memory)
        {
            vkFreeMemory(device, m_Memory, nullptr);
            m_Memory = VK_NULL_HANDLE;
        }
    }

    uint32_t Buffer::FindMemoryType(
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

        throw std::runtime_error("Failed to find suitable memory type.");
    }

    void Buffer::SetData(const void* data, VkDeviceSize size)
    {
        if (size > m_Size)
        {
            throw std::runtime_error("Buffer upload size exceeds buffer size.");
        }

        void* mappedMemory = nullptr;

        vkMapMemory(
            m_Context.getDevice(),
            m_Memory,
            0,
            size,
            0,
            &mappedMemory
        );

        std::memcpy(mappedMemory, data, static_cast<size_t>(size));

        vkUnmapMemory(m_Context.getDevice(), m_Memory);
    }
}