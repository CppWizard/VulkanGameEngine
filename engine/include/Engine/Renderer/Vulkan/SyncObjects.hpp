#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>

#include <vulkan/vulkan.h>

#include <vector>
#include <cstdint>

namespace Engine
{
    class SyncObjects
    {
    public:
        static constexpr uint32_t MaxFramesInFlight = 2;

        SyncObjects(VulkanContext& context, uint32_t swapchainImageCount);
        ~SyncObjects();

        VkSemaphore GetImageAvailableSemaphore(uint32_t frame) const;
        void Recreate(uint32_t swapchainImageCount);

        VkSemaphore GetRenderFinishedSemaphore(uint32_t imageIndex) const;
        VkFence GetInFlightFence(uint32_t frame) const;

    private:
        void Create(uint32_t swapchainImageCount);
        void Cleanup();

    private:
        VulkanContext& m_Context;

        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;
    };
}
