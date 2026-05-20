#pragma once

#include <Engine/Renderer/Vertex.hpp>

#include <filesystem>
#include <vector>
#include <string>
#include <cstdint>

namespace Engine
{
    struct ObjSubMeshData
    {
        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;

        std::string MaterialName;
        std::filesystem::path DiffuseTexturePath;
    };

    struct ObjMeshData
    {
        std::vector<ObjSubMeshData> SubMeshes;
    };

    class ObjLoader
    {
    public:
        static ObjMeshData Load(const std::filesystem::path& path);
    };
}