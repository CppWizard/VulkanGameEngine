#include <Engine/Renderer/Vulkan/CommandBuffers.hpp>
#include <Engine/Core/Logger.hpp>

#include <stdexcept>
#include <array>

namespace Engine
{
    CommandBuffers::CommandBuffers(
        VulkanContext& context,
        const CommandPool& commandPool,
        const Swapchain& swapchain
    )
        : m_Context(context), m_CommandPool(commandPool)
    {
        Allocate(swapchain);
    }

    CommandBuffers::~CommandBuffers()
    {
        Cleanup();
    }

    void CommandBuffers::Allocate(const Swapchain& swapchain)
    {
        m_CommandBuffers.resize(swapchain.GetImageCount());

        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.commandPool = m_CommandPool.GetHandle();
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount =
            static_cast<uint32_t>(m_CommandBuffers.size());

        if (vkAllocateCommandBuffers(
            m_Context.getDevice(),
            &allocateInfo,
            m_CommandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate command buffers.");
        }

		ENGINE_INFO("Command buffers allocated.");
    }

    void CommandBuffers::Cleanup()
    {
        if (!m_CommandBuffers.empty())
        {
            vkFreeCommandBuffers(
                m_Context.getDevice(),
                m_CommandPool.GetHandle(),
                static_cast<uint32_t>(m_CommandBuffers.size()),
                m_CommandBuffers.data()
            );

            m_CommandBuffers.clear();
        }
    }

    void CommandBuffers::Record(
        uint32_t imageIndex,
        uint32_t frameIndex,
        const Swapchain& swapchain,
        const RenderPass& renderPass,
        const Framebuffers& framebuffers,
        const GraphicsPipeline& pipeline,
        const Mesh& mesh,
		const DescriptorSets& descriptorSets
    )
    {
        VkCommandBuffer commandBuffer = m_CommandBuffers[imageIndex];

        vkResetCommandBuffer(commandBuffer, 0);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording command buffer.");
        }

        std::array<VkClearValue, 2> clearValues{};

        clearValues[0].color = { { 0.08f, 0.10f, 0.18f, 1.0f } };
        clearValues[1].depthStencil = { 1.0f, 0 };

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass.GetHandle();
        renderPassInfo.framebuffer = framebuffers.Get(imageIndex);
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapchain.GetExtent();
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(
            commandBuffer,
            &renderPassInfo,
            VK_SUBPASS_CONTENTS_INLINE
        );

        vkCmdBindPipeline(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipeline.GetHandle()
        );

		VkDescriptorSet descriptorSet = descriptorSets.Get(frameIndex);

        vkCmdBindDescriptorSets(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipeline.GetLayout(),
            0,
            1,
            &descriptorSet,
            0,
            nullptr
        );

        VkBuffer vertexBuffers[] =
        {
			mesh.GetVertexBuffer().GetHandle()
        };

        VkDeviceSize offsets[] =
        {
            0
        };

        vkCmdBindVertexBuffers(
            commandBuffer,
            0,
            1,
            vertexBuffers,
            offsets
        );

        vkCmdBindIndexBuffer(
            commandBuffer,
			mesh.GetIndexBuffer().GetHandle(),
            0,
            VK_INDEX_TYPE_UINT32
        );

        vkCmdDrawIndexed(
            commandBuffer,
            mesh.GetIndexCount(),
            1,
            0,
            0,
            0
        );

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to record command buffer.");
        }
    }

    void CommandBuffers::Recreate(const Swapchain& swapchain)
    {
        Cleanup();
        Allocate(swapchain);
    }
}