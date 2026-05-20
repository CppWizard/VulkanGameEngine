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
#include <Engine/Renderer/Vulkan/ImGuiLayer.hpp>

#include <Engine/Renderer/Camera.hpp>

#include <Engine/Platform/Window.hpp>

#include <Engine/Editor/EditorViewport.hpp>

#include <deque>
#include <string>

namespace Engine
{
    class VulkanRenderer
    {
    public:
        VulkanRenderer(
            VulkanContext& context,
            Window& window,
            Swapchain& swapchain,
            RenderPass& renderPass, Framebuffers& framebuffers,
            CommandBuffers& commandBuffers,
            SyncObjects& syncObjects,
            GraphicsPipeline& graphicsPipeline,
            Scene& scene,
            UniformBuffers& uniformBuffers,
            DescriptorSets& descriptorSets,
			DepthBuffer& depthBuffer,
            Camera& camera,
            ImGuiLayer* imguiLayer,
            const std::deque<std::string>& eventLog
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
        Scene& m_Scene;
        UniformBuffers& m_UniformBuffers;
        DescriptorSets& m_DescriptorSets;
        DepthBuffer& m_DepthBuffer;
        Camera& m_Camera;
        ImGuiLayer* m_ImGuiLayer = nullptr;
        EditorViewport m_Viewport;
        const std::deque<std::string>& m_EventLog;

        uint32_t m_CurrentFrame = 0;
    };
}