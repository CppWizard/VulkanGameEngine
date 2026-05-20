#pragma once

#include <Engine/Renderer/Vulkan/Texture2D.hpp>
#include <Engine/Renderer/Vulkan/MaterialDescriptorSet.hpp>

#include <memory>
#include <string>

namespace Engine
{
    struct Material
    {
        std::string Name;

        std::shared_ptr<Texture2D> DiffuseTexture;
        std::unique_ptr<MaterialDescriptorSet> DescriptorSet;
    };
}