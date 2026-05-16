#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>
#include <Engine/Renderer/Vulkan/Swapchain.hpp>
#include <Engine/Renderer/Vulkan/RenderPass.hpp>
#include <Engine/Renderer/Vulkan/ShaderModule.hpp>
#include <Engine/Renderer/Vulkan/DescriptorSetLayout.hpp>

#include <vulkan/vulkan.h>

namespace Engine
{
    class GraphicsPipeline
    {
    public:
        GraphicsPipeline(
            VulkanContext& context,
            const Swapchain& swapchain,
            const RenderPass& renderPass,
            const ShaderModule& vertexShader,
            const ShaderModule& fragmentShader,
			const DescriptorSetLayout& descriptorSetLayout
        );

        ~GraphicsPipeline();

        GraphicsPipeline(const GraphicsPipeline&) = delete;
        GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

        VkPipeline GetHandle() const { return m_Pipeline; }
        VkPipelineLayout GetLayout() const { return m_PipelineLayout; }

    private:
        void Create(
            const Swapchain& swapchain,
            const RenderPass& renderPass,
            const ShaderModule& vertexShader,
            const ShaderModule& fragmentShader,
			const DescriptorSetLayout& descriptorSetLayout
        );

        void Cleanup();

    private:
        VulkanContext& m_Context;

        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
        VkPipeline m_Pipeline = VK_NULL_HANDLE;
    };
}