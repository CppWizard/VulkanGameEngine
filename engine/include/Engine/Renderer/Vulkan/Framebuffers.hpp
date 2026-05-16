#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>
#include <Engine/Renderer/Vulkan/Swapchain.hpp>
#include <Engine/Renderer/Vulkan/RenderPass.hpp>
#include <Engine/Renderer/Vulkan/DepthBuffer.hpp>

#include <vulkan/vulkan.h>

#include <vector>

namespace Engine
{
	class Framebuffers
	{
	public:
		Framebuffers(VulkanContext& context, const Swapchain& swapchain, const RenderPass& renderPass, const DepthBuffer& depthBuffer);

		~Framebuffers();

		Framebuffers(const Framebuffers&) = delete;
		Framebuffers& operator=(const Framebuffers&) = delete;

		void Recreate(
			const Swapchain& swapchain,
			const RenderPass& renderPass,
			const DepthBuffer& depthBuffer
		);

		

		VkFramebuffer Get(uint32_t index) const
		{
			return m_Framebuffers[index];
		}

		const std::vector<VkFramebuffer>& GetAll() const
		{
			return m_Framebuffers;
		}

		uint32_t Count() const
		{
			return static_cast<uint32_t>(m_Framebuffers.size());
		}

	private:
		void Create(
			const Swapchain& swapchain,
			const RenderPass& renderPass,
			const DepthBuffer& depthBuffer
		);

		void Cleanup();

	private:
		const VulkanContext& m_Context;
		std::vector<VkFramebuffer> m_Framebuffers;
	};
	
}