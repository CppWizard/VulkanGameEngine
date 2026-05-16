#include <Engine/Renderer/Vulkan/Framebuffers.hpp>
#include <Engine/Core/Logger.hpp>

#include <array>
#include <stdexcept>

namespace Engine
{
    Framebuffers::Framebuffers(
        VulkanContext& context,
        const Swapchain& swapchain,
        const RenderPass& renderPass,
		const DepthBuffer& depthBuffer
    )
        : m_Context(context)
    {
        Create(swapchain, renderPass, depthBuffer);
    }

    Framebuffers::~Framebuffers()
    {
		Cleanup();
    }

    void Framebuffers::Recreate(
        const Swapchain& swapchain,
        const RenderPass& renderPass,
		const DepthBuffer& depthBuffer
    )
    {
        vkDeviceWaitIdle(m_Context.getDevice());

        Cleanup();
        Create(swapchain, renderPass, depthBuffer);
    }

    void Framebuffers::Create(
        const Swapchain& swapchain,
        const RenderPass& renderPass,
		const DepthBuffer& depthBuffer
    )
    {
        const auto& imageViews = swapchain.GetImageViews();

        m_Framebuffers.resize(imageViews.size());

        for (size_t i = 0; i < imageViews.size(); i++)
        {
            std::array<VkImageView, 2> attachments =
            {
                imageViews[i],
                depthBuffer.GetImageView()
            };

            VkFramebufferCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            createInfo.renderPass = renderPass.GetHandle();
            createInfo.attachmentCount =
                static_cast<uint32_t>(attachments.size());
            createInfo.pAttachments = attachments.data();
            createInfo.width = swapchain.GetExtent().width;
            createInfo.height = swapchain.GetExtent().height;
            createInfo.layers = 1;

            if (vkCreateFramebuffer(
                m_Context.getDevice(),
                &createInfo,
                nullptr,
                &m_Framebuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create framebuffer.");
            }
        }

        ENGINE_INFO("Framebuffers created.");
    }

    void Framebuffers::Cleanup()
    {
        for (VkFramebuffer framebuffer : m_Framebuffers)
        {
            vkDestroyFramebuffer(
                m_Context.getDevice(),
                framebuffer,
                nullptr
            );
        }

        m_Framebuffers.clear();

        ENGINE_INFO("Framebuffers destroyed.");
    }
}