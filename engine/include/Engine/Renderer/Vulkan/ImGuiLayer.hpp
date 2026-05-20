#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>
#include <Engine/Renderer/Vulkan/Swapchain.hpp>
#include <Engine/Renderer/Vulkan/RenderPass.hpp>
#include <Engine/Renderer/Vulkan/CommandPool.hpp>
#include <Engine/Platform/Window.hpp>

#include <vulkan/vulkan.h>

namespace Engine
{
    class ImGuiLayer
    {
    public:
        ImGuiLayer(
            VulkanContext& context,
            Window& window,
            const Swapchain& swapchain,
            const RenderPass& renderPass,
            const CommandPool& commandPool
        );

        ~ImGuiLayer();

        void BeginFrame();
        void EndFrame();

        void Render(VkCommandBuffer commandBuffer);
        void UpdatePlatformWindows();

    private:
        void CreateDescriptorPool();
        void Shutdown();

    private:
        VulkanContext& m_Context;
        VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
    };
}
