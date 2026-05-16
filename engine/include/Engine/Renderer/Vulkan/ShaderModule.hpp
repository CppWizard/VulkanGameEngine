#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>

#include <vulkan/vulkan.h>

#include <filesystem>
#include <vector>
#include <cstdint>

namespace Engine
{
    class ShaderModule
    {
    public:
        ShaderModule(
            VulkanContext& context,
            const std::filesystem::path& path
        );

        ~ShaderModule();

        ShaderModule(const ShaderModule&) = delete;
        ShaderModule& operator=(const ShaderModule&) = delete;

        VkShaderModule GetHandle() const
        {
            return m_ShaderModule;
        }

        VkPipelineShaderStageCreateInfo GetStageCreateInfo(
            VkShaderStageFlagBits stage,
            const char* entryPoint = "main"
        ) const;

        static std::vector<uint32_t> ReadSpirvFile(
            const std::filesystem::path& path
        );

    private:
        void Create(const std::vector<uint32_t>& code);
        void Cleanup();

    private:
        VulkanContext& m_Context;
        VkShaderModule m_ShaderModule = VK_NULL_HANDLE;
    };
}