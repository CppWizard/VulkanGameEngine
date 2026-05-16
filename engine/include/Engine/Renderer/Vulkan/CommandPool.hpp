#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>

#include <vulkan/vulkan.h>

#include <vector>

namespace Engine
{
	class CommandPool
	{
	public:
		explicit CommandPool(const VulkanContext& context);
		~CommandPool();

		CommandPool(const CommandPool&) = delete;
		CommandPool& operator=(const CommandPool&) = delete;

		VkCommandPool GetHandle() const { return m_CommandPool; }

		VkCommandBuffer AllocateCommandBuffer() const;
		std::vector<VkCommandBuffer> AllocateCommandBuffers(uint32_t count) const;

		VkCommandBuffer BeginSingleTimeCommands() const;
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer) const;

	private:
		void Create();
		void Cleanup();

	private:
		const VulkanContext& m_Context;
		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
	};
}