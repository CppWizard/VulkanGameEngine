#include <Engine/Renderer/Vulkan/VulkanRenderer.hpp>
#include <Engine/Renderer/UniformBufferObject.hpp>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>
#include <array>

namespace Engine
{
    VulkanRenderer::VulkanRenderer(
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
    )
        : m_Context(context),
        m_Window(window),
        m_Swapchain(swapchain),
        m_RenderPass(renderPass),
        m_Framebuffers(framebuffers),
        m_CommandBuffers(commandBuffers),
		m_SyncObjects(syncObjects),
		m_GraphicsPipeline(graphicsPipeline),
        m_Mesh(mesh),
		m_UniformBuffers(uniformBuffers),
		m_DescriptorSets(descriptorSets),
		m_DepthBuffer(depthBuffer),
		m_Camera(camera)
    {}

    void VulkanRenderer::DrawFrame()
    {
        VkDevice device = m_Context.getDevice();

        VkFence inFlightFence =
            m_SyncObjects.GetInFlightFence(m_CurrentFrame);

        vkWaitForFences(
            device,
            1,
            &inFlightFence,
            VK_TRUE,
            UINT64_MAX
        );

        uint32_t imageIndex = 0;

        VkSemaphore imageAvailableSemaphore =
            m_SyncObjects.GetImageAvailableSemaphore(m_CurrentFrame);

        VkResult acquireResult = vkAcquireNextImageKHR(
            device,
            m_Swapchain.GetHandle(),
            UINT64_MAX,
            imageAvailableSemaphore,
            VK_NULL_HANDLE,
            &imageIndex
        );

        if (acquireResult != VK_SUCCESS &&
            acquireResult != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("Failed to acquire swapchain image.");
        }

        vkResetFences(device, 1, &inFlightFence);

        UniformBufferObject ubo{};
        ubo.Model =
            glm::rotate(
                glm::mat4(1.0f),
                static_cast<float>(glfwGetTime()) *
                glm::radians(90.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)
            );

        ubo.View = m_Camera.GetViewMatrix();
        ubo.Projection = m_Camera.GetProjectionMatrix();

        m_UniformBuffers.Update(m_CurrentFrame, ubo);

        m_CommandBuffers.Record(
            imageIndex,
            m_CurrentFrame,
            m_Swapchain,
            m_RenderPass,
            m_Framebuffers,
			m_GraphicsPipeline,
            m_Mesh,
			m_DescriptorSets
        );

        VkSemaphore renderFinishedSemaphore =
            m_SyncObjects.GetRenderFinishedSemaphore(imageIndex);

        VkCommandBuffer commandBuffer =
            m_CommandBuffers.Get(imageIndex);

        VkPipelineStageFlags waitStages[] =
        {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
        };

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &imageAvailableSemaphore;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &renderFinishedSemaphore;

        if (vkQueueSubmit(
            m_Context.getGraphicsQueue(),
            1,
            &submitInfo,
            inFlightFence) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to submit draw command buffer.");
        }

        VkSwapchainKHR swapchains[] =
        {
            m_Swapchain.GetHandle()
        };

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &renderFinishedSemaphore;

        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapchains;
        presentInfo.pImageIndices = &imageIndex;

        VkResult presentResult = vkQueuePresentKHR(
            m_Context.getPresentQueue(),
            &presentInfo
        );

        if (presentResult != VK_SUCCESS &&
            presentResult != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("Failed to present swapchain image.");
        }

        m_CurrentFrame =
            (m_CurrentFrame + 1) % SyncObjects::MaxFramesInFlight;
    }

    void VulkanRenderer::RecreateSwapchain()
    {
        while (m_Window.Width() == 0 || m_Window.Height() == 0)
        {
            m_Window.PollEvents();
        }

        vkDeviceWaitIdle(m_Context.getDevice());

        m_Swapchain.Recreate(m_Window);
        m_SyncObjects.Recreate(m_Swapchain.GetImageCount());
        m_CommandBuffers.Recreate(m_Swapchain);
        m_Framebuffers.Recreate(m_Swapchain, m_RenderPass, m_DepthBuffer);
        m_CommandBuffers.Recreate(m_Swapchain);

        m_Window.ResetResizeFlag();

        m_Camera.SetAspectRatio(
            static_cast<float>(m_Swapchain.GetExtent().width) /
            static_cast<float>(m_Swapchain.GetExtent().height)
        );
    }
}
