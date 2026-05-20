#include <Engine/Renderer/Vulkan/VulkanRenderer.hpp>
#include <Engine/Renderer/UniformBufferObject.hpp>
#include <Engine/Core/Input.hpp>

#include <GLFW/glfw3.h>
#include <imgui.h>

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
        Scene& scene,
        UniformBuffers& uniformBuffers,
        DescriptorSets& descriptorSets,
        DepthBuffer& depthBuffer,
        Camera& camera,
        ImGuiLayer* imguiLayer,
        const std::deque<std::string>& eventLog
    )
        : m_Context(context),
        m_Window(window),
        m_Swapchain(swapchain),
        m_RenderPass(renderPass),
        m_Framebuffers(framebuffers),
        m_CommandBuffers(commandBuffers),
        m_SyncObjects(syncObjects),
        m_GraphicsPipeline(graphicsPipeline),
        m_Scene(scene),
		m_UniformBuffers(uniformBuffers),
		m_DescriptorSets(descriptorSets),
		m_DepthBuffer(depthBuffer),
		m_Camera(camera),
		m_ImGuiLayer(imguiLayer),
        m_EventLog(eventLog)
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

        if (m_ImGuiLayer)
        {
            m_ImGuiLayer->BeginFrame();

            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    ImGui::MenuItem("Exit");
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("View"))
                {
                    ImGui::MenuItem("Input Debug");
                    ImGui::MenuItem("Scene Hierarchy");
                    ImGui::MenuItem("Inspector");
                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }

            m_Viewport.Draw();

            ImGui::Begin("Input Debug");
            ImGui::Text("Mouse Position: %.1f, %.1f", Input::GetMouseX(), Input::GetMouseY());
            ImGui::Text("Mouse Delta: %.1f, %.1f", Input::GetMouseDeltaX(), Input::GetMouseDeltaY());
            ImGui::Separator();
            ImGui::Text("W: %s", Input::IsKeyDown(GLFW_KEY_W) ? "Down" : "Up");
            ImGui::Text("A: %s", Input::IsKeyDown(GLFW_KEY_A) ? "Down" : "Up");
            ImGui::Text("S: %s", Input::IsKeyDown(GLFW_KEY_S) ? "Down" : "Up");
            ImGui::Text("D: %s", Input::IsKeyDown(GLFW_KEY_D) ? "Down" : "Up");
            ImGui::End();

            ImGui::Begin("Scene Hierarchy");
            ImGui::Text("Scene");
            ImGui::BulletText("Quad");
            ImGui::End();

            static float position[3] = { 0.0f, 0.0f, 0.0f };

            ImGui::Begin("Inspector");
            ImGui::Text("Selected: Quad");
            ImGui::DragFloat3("Position", position);
            ImGui::Text("Viewport Focused: %s", m_Viewport.IsFocused() ? "true" : "false");
            ImGui::Text("Viewport Hovered: %s", m_Viewport.IsHovered() ? "true" : "false");
            ImGui::End();

            ImGui::Begin("Event Log");

            if (ImGui::Button("Clear"))
            {
                // make this editable later
            }

            ImGui::Separator();

            for (const auto& eventText : m_EventLog)
            {
                ImGui::TextWrapped("%s", eventText.c_str());
            }

            ImGui::End();

            m_ImGuiLayer->EndFrame();
        }

        UniformBufferObject ubo{};
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
            m_Scene,
			m_DescriptorSets,
			m_ImGuiLayer
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

        if (m_ImGuiLayer)
        {
            m_ImGuiLayer->UpdatePlatformWindows();
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


        m_Window.ResetResizeFlag();

        m_Camera.SetAspectRatio(
            static_cast<float>(m_Swapchain.GetExtent().width) /
            static_cast<float>(m_Swapchain.GetExtent().height)
        );
    }
}
