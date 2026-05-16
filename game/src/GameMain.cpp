#include <Engine/Engine.hpp>

#include <GLFW/glfw3.h>

#include <memory>

class GameApp final : public Engine::Application
{
public:
    GameApp()
        : Engine::Application({
            .Title = "Vulkan Game",
            .Width = 1280,
            .Height = 720,
            .Resizable = true
            })
    {}

protected:
    void OnStart() override
    {
        ENGINE_INFO("Game started.");

        m_VulkanContext =
            std::make_unique<Engine::VulkanContext>(GetWindow());

        m_Swapchain =
            std::make_unique<Engine::Swapchain>(*m_VulkanContext, GetWindow());

        m_DepthBuffer =
            std::make_unique<Engine::DepthBuffer>(
                *m_VulkanContext,
                *m_Swapchain
            );

        m_RenderPass =
            std::make_unique<Engine::RenderPass>(
                *m_VulkanContext,
                *m_Swapchain,
                m_DepthBuffer->GetFormat()
            );

        m_VertexShader =
            std::make_unique<Engine::ShaderModule>(
                *m_VulkanContext,
                "C:/EngineDev/VulkanGameEngine/build/bin/shaders/triangle.vert.spv"
            );

        m_FragmentShader =
            std::make_unique<Engine::ShaderModule>(
                *m_VulkanContext,
                "C:/EngineDev/VulkanGameEngine/build/bin/shaders/triangle.frag.spv"
            );

        m_DescriptorSetLayout =
            std::make_unique<Engine::DescriptorSetLayout>(*m_VulkanContext);

        m_UniformBuffers =
            std::make_unique<Engine::UniformBuffers>(*m_VulkanContext);

        m_DescriptorPool =
            std::make_unique<Engine::DescriptorPool>(*m_VulkanContext);

        // Command pool and texture must be created before descriptor sets
        // because DescriptorSets uses the texture when being constructed.
        m_CommandPool =
            std::make_unique<Engine::CommandPool>(*m_VulkanContext);

        m_Texture =
            std::make_unique<Engine::Texture2D>(
                *m_VulkanContext,
                *m_CommandPool,
                "C:/EngineDev/VulkanGameEngine/assets/textures/checker.png"
            );

        m_DescriptorSets =
            std::make_unique<Engine::DescriptorSets>(
                *m_VulkanContext,
                *m_DescriptorPool,
                *m_DescriptorSetLayout,
                *m_UniformBuffers,
                *m_Texture
            );

        m_GraphicsPipeline =
            std::make_unique<Engine::GraphicsPipeline>(
                *m_VulkanContext,
                *m_Swapchain,
                *m_RenderPass,
                *m_VertexShader,
                *m_FragmentShader,
				*m_DescriptorSetLayout
            );

        std::vector<Engine::Vertex> vertices =
        {
            { { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
            { {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
            { {  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
            { { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }
        };

        std::vector<uint32_t> indices =
        {
            0, 1, 2,
            2, 3, 0
        };

        m_Mesh =
            std::make_unique<Engine::Mesh>(
                *m_VulkanContext,
                vertices,
                indices
            );

        m_Framebuffers =
            std::make_unique<Engine::Framebuffers>(
                *m_VulkanContext,
                *m_Swapchain,
                *m_RenderPass,
				*m_DepthBuffer
            );

        m_CommandBuffers =
            std::make_unique<Engine::CommandBuffers>(
                *m_VulkanContext,
                *m_CommandPool,
                *m_Swapchain
            );

        m_SyncObjects =
            std::make_unique<Engine::SyncObjects>(
                *m_VulkanContext,
                m_Swapchain->GetImageCount()
            );

        m_Camera =
            std::make_unique<Engine::Camera>(
                45.0f,
                static_cast<float>(m_Swapchain->GetExtent().width) /
                static_cast<float>(m_Swapchain->GetExtent().height),
                0.1f,
                100.0f
            );

        m_Camera->SetPosition({ 0.0f, 0.0f, 2.0f });
        m_Camera->SetTarget({ 0.0f, 0.0f, 0.0f });

        m_Renderer =
            std::make_unique<Engine::VulkanRenderer>(
                *m_VulkanContext,
                GetWindow(),
                *m_Swapchain,
                *m_RenderPass,
                *m_Framebuffers,
                *m_CommandBuffers,
                *m_SyncObjects,
				*m_GraphicsPipeline,
                *m_Mesh,
                *m_UniformBuffers,
                *m_DescriptorSets,
				*m_DepthBuffer,
				*m_Camera
            );
    }

    void OnUpdate(float deltaTime) override
    {
        (void)deltaTime;

        if (Engine::Input::IsKeyPressed(GetWindow(), GLFW_KEY_ESCAPE))
        {
            Close();
        }
    }

    void OnRender() override
    {
        m_Renderer->DrawFrame();
    }

    void OnShutdown() override
    {
        vkDeviceWaitIdle(m_VulkanContext->getDevice());

        m_Renderer.reset();
        m_Camera.reset();
        m_Mesh.reset();
        m_SyncObjects.reset();
        m_CommandBuffers.reset();

        m_Framebuffers.reset();
        m_GraphicsPipeline.reset();

        m_DescriptorSets.reset();
        m_DescriptorPool.reset();
        m_Texture.reset();
        m_UniformBuffers.reset();
        m_DescriptorSetLayout.reset();
        m_FragmentShader.reset();
        m_CommandPool.reset();
        m_VertexShader.reset();
        m_RenderPass.reset();
        m_Swapchain.reset();
        m_VulkanContext.reset();
        m_DepthBuffer.reset();

        ENGINE_INFO("Game shutdown.");
    }

private:

    std::unique_ptr<Engine::VulkanContext> m_VulkanContext;
    std::unique_ptr<Engine::Swapchain> m_Swapchain;
    std::unique_ptr<Engine::RenderPass> m_RenderPass;
    std::unique_ptr<Engine::Framebuffers> m_Framebuffers;
    std::unique_ptr<Engine::CommandPool> m_CommandPool;
    std::unique_ptr<Engine::CommandBuffers> m_CommandBuffers;
    std::unique_ptr<Engine::SyncObjects> m_SyncObjects;
    std::unique_ptr<Engine::VulkanRenderer> m_Renderer;
    std::unique_ptr<Engine::ShaderModule> m_VertexShader;
    std::unique_ptr<Engine::ShaderModule> m_FragmentShader;
    std::unique_ptr<Engine::GraphicsPipeline> m_GraphicsPipeline;
    std::unique_ptr<Engine::Mesh> m_Mesh;
    std::unique_ptr<Engine::DescriptorSetLayout> m_DescriptorSetLayout;
    std::unique_ptr<Engine::UniformBuffers> m_UniformBuffers;
    std::unique_ptr<Engine::DescriptorPool> m_DescriptorPool;
    std::unique_ptr<Engine::DescriptorSets> m_DescriptorSets;
    std::unique_ptr<Engine::DepthBuffer> m_DepthBuffer;
    std::unique_ptr<Engine::Camera> m_Camera;
    std::unique_ptr<Engine::Texture2D> m_Texture;
};

int main()
{
    GameApp app;
    app.Run();

    return 0;
}
