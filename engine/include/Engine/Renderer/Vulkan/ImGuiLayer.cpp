#include <Engine/Renderer/Vulkan/ImGuiLayer.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include <iterator>
#include <stdexcept>

namespace Engine
{
    ImGuiLayer::ImGuiLayer(
        VulkanContext& context,
        Window& window,
        const Swapchain& swapchain,
        const RenderPass& renderPass,
        const CommandPool&
    )
        : m_Context(context)
    {
        CreateDescriptorPool();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForVulkan(window.NativeHandle(), true);

        ImGui_ImplVulkan_InitInfo initInfo{};
        initInfo.Instance = m_Context.getInstance();
        initInfo.PhysicalDevice = m_Context.getPhysicalDevice();
        initInfo.Device = m_Context.getDevice();
        initInfo.QueueFamily = m_Context.GetQueueFamilies().GraphicsFamily.value();
        initInfo.Queue = m_Context.getGraphicsQueue();
        initInfo.PipelineCache = VK_NULL_HANDLE;
        initInfo.DescriptorPool = m_DescriptorPool;
        initInfo.MinImageCount = swapchain.GetImageCount();
        initInfo.ImageCount = swapchain.GetImageCount();
        initInfo.PipelineInfoMain.RenderPass = renderPass.GetHandle();
        initInfo.PipelineInfoMain.Subpass = 0;
        initInfo.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        initInfo.Allocator = nullptr;
        initInfo.CheckVkResultFn = nullptr;

        ImGui_ImplVulkan_Init(&initInfo);
    }

    ImGuiLayer::~ImGuiLayer()
    {
        Shutdown();
    }

    void ImGuiLayer::CreateDescriptorPool()
    {
        VkDescriptorPoolSize poolSizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000 * static_cast<uint32_t>(std::size(poolSizes));
        poolInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
        poolInfo.pPoolSizes = poolSizes;

        if (vkCreateDescriptorPool(
            m_Context.getDevice(),
            &poolInfo,
            nullptr,
            &m_DescriptorPool) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create ImGui descriptor pool.");
        }
    }

    void ImGuiLayer::BeginFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(
            0,
            ImGui::GetMainViewport(),
            ImGuiDockNodeFlags_PassthruCentralNode
        );
    }

    void ImGuiLayer::EndFrame()
    {
        ImGui::Render();
    }

    void ImGuiLayer::Render(VkCommandBuffer commandBuffer)
    {
        ImGui_ImplVulkan_RenderDrawData(
            ImGui::GetDrawData(),
            commandBuffer
        );
    }

    void ImGuiLayer::UpdatePlatformWindows()
    {
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void ImGuiLayer::Shutdown()
    {
        vkDeviceWaitIdle(m_Context.getDevice());

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        if (m_DescriptorPool)
        {
            vkDestroyDescriptorPool(
                m_Context.getDevice(),
                m_DescriptorPool,
                nullptr
            );

            m_DescriptorPool = VK_NULL_HANDLE;
        }
    }
}
