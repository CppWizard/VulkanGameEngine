#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>
#include <Engine/Renderer/Vulkan/SwapChain.hpp>

#include <Vulkan/vulkan.h>

namespace Engine
{
	class RenderPass
	{
	public:
		RenderPass(const VulkanContext& context, const Swapchain& swapchain, VkFormat depthFormat);
		~RenderPass();

		RenderPass(const RenderPass&) = delete;
		RenderPass& operator=(const RenderPass&) = delete;

		VkRenderPass GetHandle() const { return m_RenderPass; }

	private:
		void Create(const Swapchain& swapchain, VkFormat depthFormat);
		void Cleanup();

	private:
		const VulkanContext& m_Context;
		VkRenderPass m_RenderPass = VK_NULL_HANDLE;

	};
}