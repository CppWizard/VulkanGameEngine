#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include <array>
#include <cstddef>

namespace Engine
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Color;
        glm::vec2 TexCoord;

        bool operator==(const Vertex& other) const
        {
            return Position == other.Position &&
                Color == other.Color &&
                TexCoord == other.TexCoord;
        }

        static VkVertexInputBindingDescription GetBindingDescription()
        {
            VkVertexInputBindingDescription binding{};
            binding.binding = 0;
            binding.stride = sizeof(Vertex);
            binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return binding;
        }

        static std::array<VkVertexInputAttributeDescription, 3>
            GetAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 3> attributes{};

            attributes[0].binding = 0;
            attributes[0].location = 0;
            attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributes[0].offset = offsetof(Vertex, Position);

            attributes[1].binding = 0;
            attributes[1].location = 1;
            attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributes[1].offset = offsetof(Vertex, Color);

            attributes[2].binding = 0;
            attributes[2].location = 2;
            attributes[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributes[2].offset = offsetof(Vertex, TexCoord);

            return attributes;
        }
    };
}