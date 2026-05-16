#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>
#include <Engine/Renderer/Vulkan/Swapchain.hpp>
#include <Engine/Renderer/Vulkan/RenderPass.hpp>
#include <Engine/Renderer/Vulkan/Framebuffers.hpp>
#include <Engine/Renderer/Vulkan/CommandBuffers.hpp>
#include <Engine/Renderer/Vulkan/SyncObjects.hpp>
#include <Engine/Renderer/Vulkan/GraphicsPipeline.hpp>
#include <Engine/Renderer/Mesh.hpp>
#include <Engine/Renderer/Vulkan/DepthBuffer.hpp>

#include <Engine/Renderer/Camera.hpp>

#include <Engine/Platform/Window.hpp>

namespace Engine
{
    class VulkanRenderer
    {
    public:
        VulkanRenderer(
            VulkanContext& context,
            Window& window,
            Swapchain& swapchain,
            RenderPass& renderPass,
            Framebuffers& framebuffers,
            CommandBuffers& commandBuffers,
            SyncObjects& syncObjects,
            GraphicsPipeline& graphicsPipeline,
            Mesh& mesh,
            UniformBuffers& uniformBuffers,
            DescriptorSets& descriptorSets,
			DepthBuffer& depthBuffer,
            Camera& camera
        );

        void DrawFrame();

        void RecreateSwapchain();

        uint32_t GetCurrentFrame() const { return m_CurrentFrame; }

    private:
        VulkanContext& m_Context;
        Window& m_Window;
        Swapchain& m_Swapchain;
        RenderPass& m_RenderPass;
        Framebuffers& m_Framebuffers;
        CommandBuffers& m_CommandBuffers;
        SyncObjects& m_SyncObjects;
        GraphicsPipeline& m_GraphicsPipeline;
        Mesh& m_Mesh;
        UniformBuffers& m_UniformBuffers;
        DescriptorSets& m_DescriptorSets;
        DepthBuffer& m_DepthBuffer;
        Camera& m_Camera;

        uint32_t m_CurrentFrame = 0;
    };
}