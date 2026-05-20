#pragma once

#include <Engine/Renderer/Mesh.hpp>
#include <Engine/Renderer/Vulkan/DescriptorPool.hpp>
#include <Engine/Renderer/Vulkan/MaterialDescriptorSetLayout.hpp>
#include <filesystem>
#include <memory>

namespace Engine
{
    class MeshFactory
    {
    public:
        static std::shared_ptr<Mesh> LoadObj(
            VulkanContext& context,
            const CommandPool& commandPool,
			const DescriptorPool& descriptorPool,
			const MaterialDescriptorSetLayout& materialDescriptorSetLayout,
            const std::filesystem::path& path,
            const std::filesystem::path& fallbackTexture
        );
    };
}