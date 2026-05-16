#include <Engine/Renderer/Vulkan/ShaderModule.hpp>
#include <Engine/Core/Logger.hpp>

#include <fstream>
#include <stdexcept>

namespace Engine
{
    ShaderModule::ShaderModule(
        VulkanContext& context,
        const std::filesystem::path& path
    )
        : m_Context(context)
    {
        auto code = ReadSpirvFile(path);
        Create(code);

        ENGINE_INFO("Shader module loaded: " << path.string());
    }

    ShaderModule::~ShaderModule()
    {
        Cleanup();
    }

    std::vector<uint32_t> ShaderModule::ReadSpirvFile(
        const std::filesystem::path& path
    )
    {
        std::ifstream file(path, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error(
                "Failed to open shader file: " + path.string()
            );
        }

        const std::streamsize fileSize = file.tellg();

        if (fileSize <= 0 || fileSize % 4 != 0)
        {
            throw std::runtime_error(
                "Invalid SPIR-V shader size: " + path.string()
            );
        }

        std::vector<uint32_t> buffer(
            static_cast<size_t>(fileSize) / sizeof(uint32_t)
        );

        file.seekg(0);
        file.read(
            reinterpret_cast<char*>(buffer.data()),
            fileSize
        );

        file.close();

        return buffer;
    }

    void ShaderModule::Create(const std::vector<uint32_t>& code)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size() * sizeof(uint32_t);
        createInfo.pCode = code.data();

        if (vkCreateShaderModule(
            m_Context.getDevice(),
            &createInfo,
            nullptr,
            &m_ShaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create shader module.");
        }
    }

    void ShaderModule::Cleanup()
    {
        if (m_ShaderModule)
        {
            vkDestroyShaderModule(
                m_Context.getDevice(),
                m_ShaderModule,
                nullptr
            );

            m_ShaderModule = VK_NULL_HANDLE;
        }
    }

    VkPipelineShaderStageCreateInfo ShaderModule::GetStageCreateInfo(
        VkShaderStageFlagBits stage,
        const char* entryPoint
    ) const
    {
        VkPipelineShaderStageCreateInfo stageInfo{};
        stageInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stageInfo.stage = stage;
        stageInfo.module = m_ShaderModule;
        stageInfo.pName = entryPoint;

        return stageInfo;
    }
}