#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>
#include <Engine/Renderer/Vulkan/CommandPool.hpp>
#include <Engine/Renderer/Vulkan/Swapchain.hpp>
#include <Engine/Renderer/Vulkan/RenderPass.hpp>
#include <Engine/Renderer/Vulkan/Framebuffers.hpp>
#include <Engine/Renderer/Vulkan/GraphicsPipeline.hpp>
#include <Engine/Renderer/Vulkan/VertexBuffer.hpp>
#include <Engine/Renderer/Vulkan/DescriptorSets.hpp>
#include <Engine/Renderer/Vulkan/IndexBuffer.hpp>
#include <Engine/Renderer/Vulkan/ImGuiLayer.hpp>
#include <Engine/Renderer/PushConstants.hpp>

#include <Engine/Renderer/Mesh.hpp>
#include <Engine/Scene/Scene.hpp>

#include <vulkan/vulkan.h>

#include <vector>

namespace Engine
{
    class CommandBuffers
    {
    public:
        CommandBuffers(
            VulkanContext& context,
            const CommandPool& commandPool,
            const Swapchain& swapchain
        );

        ~CommandBuffers();

        CommandBuffers(const CommandBuffers&) = delete;
        CommandBuffers& operator=(const CommandBuffers&) = delete;

        VkCommandBuffer Get(uint32_t index) const
        {
            return m_CommandBuffers[index];
        }

        const std::vector<VkCommandBuffer>& GetAll() const
        {
            return m_CommandBuffers;
        }

        uint32_t Count() const
        {
            return static_cast<uint32_t>(m_CommandBuffers.size());
        }

        void Record(
            uint32_t imageIndex,
            uint32_t frameIndex,
            const Swapchain& swapchain,
            const RenderPass& renderPass,
            const Framebuffers& framebuffers,
            const GraphicsPipeline& pipeline,
            const Scene& scene,
			const DescriptorSets& descriptorSets,
			ImGuiLayer* imguiLayer
        );

        void Recreate(const Swapchain& swapchain);

    private:
        void Allocate(const Swapchain& swapchain);
        void Cleanup();

    private:
        VulkanContext& m_Context;
        const CommandPool& m_CommandPool;

        std::vector<VkCommandBuffer> m_CommandBuffers;
    };
}