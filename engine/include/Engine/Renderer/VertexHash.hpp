#pragma once

#include <Engine/Renderer/Vertex.hpp>

#include <functional>

namespace std
{
    template<>
    struct hash<Engine::Vertex>
    {
        size_t operator()(const Engine::Vertex& vertex) const
        {
            size_t h1 = hash<float>()(vertex.Position.x) ^
                hash<float>()(vertex.Position.y) ^
                hash<float>()(vertex.Position.z);

            size_t h2 = hash<float>()(vertex.Color.x) ^
                hash<float>()(vertex.Color.y) ^
                hash<float>()(vertex.Color.z);

            size_t h3 = hash<float>()(vertex.TexCoord.x) ^
                hash<float>()(vertex.TexCoord.y);

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}