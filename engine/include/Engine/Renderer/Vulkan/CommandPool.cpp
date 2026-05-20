#include <Engine/Renderer/Vulkan/CommandPool.hpp>
#include <Engine/Core/Logger.hpp>

#include <stdexcept>

namespace Engine
{
	CommandPool::CommandPool(const VulkanContext& context)
		: m_Context(context)
	{
		Create();
	}

	CommandPool::~CommandPool()
	{
		Cleanup();
	}

	void CommandPool::Create()
	{
		const QueueFamilyIndices& queueFamilyIndices = m_Context.GetQueueFamilies();

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

		if (vkCreateCommandPool(
			m_Context.getDevice(),
			&poolInfo,
			nullptr,
			&m_CommandPool) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create command pool!");
		}

		if (m_CommandPool == VK_NULL_HANDLE)
		{
			throw std::runtime_error("Command pool is null after creation.");
		}

		ENGINE_INFO("Command pool created.");
	}

	void CommandPool::Cleanup()
	{
		if (m_CommandPool)
		{
			vkDestroyCommandPool(
				m_Context.getDevice(),
				m_CommandPool,
				nullptr
			);

			m_CommandPool = VK_NULL_HANDLE;
		}

		ENGINE_INFO("Command pool destroyed.");
	}

	VkCommandBuffer CommandPool::AllocateCommandBuffer() const
	{
		auto buffers = AllocateCommandBuffers(1);
		return buffers[0];
	}

	std::vector<VkCommandBuffer> CommandPool::AllocateCommandBuffers(uint32_t count) const
	{
		std::vector<VkCommandBuffer> commandBuffers(count);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = count;

		if (vkAllocateCommandBuffers(
			m_Context.getDevice(),
			&allocInfo,
			commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffers!");
		}

		return commandBuffers;
	}

	VkCommandBuffer CommandPool::BeginSingleTimeCommands() const
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(m_Context.getDevice(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void CommandPool::EndSingleTimeCommands(VkCommandBuffer commandBuffer) const
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(m_Context.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(m_Context.getGraphicsQueue());

		vkFreeCommandBuffers(
			m_Context.getDevice(),
			m_CommandPool,
			1,
			&commandBuffer
		);
	}
	
}