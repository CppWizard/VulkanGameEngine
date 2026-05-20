#include <Engine/Renderer/Vulkan/SyncObjects.hpp>
#include <Engine/Core/Logger.hpp>

#include <stdexcept>

namespace Engine
{
    SyncObjects::SyncObjects(VulkanContext& context, uint32_t swapchainImageCount)
        : m_Context(context)
    {
        Create(swapchainImageCount);
    }

    SyncObjects::~SyncObjects()
    {
        Cleanup();
    }

    void SyncObjects::Create(uint32_t swapchainImageCount)
    {
        m_ImageAvailableSemaphores.resize(MaxFramesInFlight);
        m_RenderFinishedSemaphores.resize(swapchainImageCount);
        m_InFlightFences.resize(MaxFramesInFlight);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (uint32_t i = 0; i < MaxFramesInFlight; i++)
        {
            if (vkCreateSemaphore(
                m_Context.getDevice(),
                &semaphoreInfo,
                nullptr,
                &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(
                    m_Context.getDevice(),
                    &fenceInfo,
                    nullptr,
                    &m_InFlightFences[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create synchronization objects.");
            }
        }

        for (uint32_t i = 0; i < swapchainImageCount; i++)
        {
            if (vkCreateSemaphore(
                m_Context.getDevice(),
                &semaphoreInfo,
                nullptr,
                &m_RenderFinishedSemaphores[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create synchronization objects.");
            }
        }

        ENGINE_INFO("Sync objects created.");
    }

    void SyncObjects::Cleanup()
    {
        VkDevice device = m_Context.getDevice();

        for (VkSemaphore semaphore : m_ImageAvailableSemaphores)
        {
            if (semaphore)
            {
                vkDestroySemaphore(
                    device,
                    semaphore,
                    nullptr
                );
            }
        }

        for (VkSemaphore semaphore : m_RenderFinishedSemaphores)
        {
            if (semaphore)
            {
                vkDestroySemaphore(
                    device,
                    semaphore,
                    nullptr
                );
            }
        }

        for (VkFence fence : m_InFlightFences)
        {
            if (fence)
            {
                vkDestroyFence(
                    device,
                    fence,
                    nullptr
                );
            }
        }

        m_ImageAvailableSemaphores.clear();
        m_RenderFinishedSemaphores.clear();
        m_InFlightFences.clear();

        ENGINE_INFO("Sync objects destroyed.");
    }

    void SyncObjects::Recreate(uint32_t swapchainImageCount)
    {
        Cleanup();
        Create(swapchainImageCount);
    }

    VkSemaphore SyncObjects::GetImageAvailableSemaphore(uint32_t frame) const
    {
        return m_ImageAvailableSemaphores[frame];
    }

    VkSemaphore SyncObjects::GetRenderFinishedSemaphore(uint32_t imageIndex) const
    {
        return m_RenderFinishedSemaphores[imageIndex];
    }

    VkFence SyncObjects::GetInFlightFence(uint32_t frame) const
    {
        return m_InFlightFences[frame];
    }
}
