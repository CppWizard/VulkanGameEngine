#include <Engine/Assets/ObjLoader.hpp>
#include <Engine/Renderer/VertexHash.hpp>

#include <tiny_obj_loader.h>

#include <unordered_map>
#include <map>
#include <stdexcept>

namespace Engine
{
    ObjMeshData ObjLoader::Load(const std::filesystem::path& path)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::string warning;
        std::string error;

        std::filesystem::path parentPath = path.parent_path();

        bool result = tinyobj::LoadObj(
            &attrib,
            &shapes,
            &materials,
            &warning,
            &error,
            path.string().c_str(),
            parentPath.string().c_str(),
            true
        );

        if (!error.empty())
            throw std::runtime_error(error);

        if (!result)
            throw std::runtime_error("Failed to load OBJ file: " + path.string());

        ObjMeshData meshData;

        std::map<int, ObjSubMeshData> subMeshesByMaterial;
        std::map<int, std::unordered_map<Vertex, uint32_t>> uniqueVerticesByMaterial;

        for (const auto& shape : shapes)
        {
            size_t indexOffset = 0;

            for (size_t face = 0; face < shape.mesh.num_face_vertices.size(); face++)
            {
                int materialId = -1;

                if (!shape.mesh.material_ids.empty())
                    materialId = shape.mesh.material_ids[face];

                auto& subMesh = subMeshesByMaterial[materialId];
                auto& uniqueVertices = uniqueVerticesByMaterial[materialId];

                const int faceVertexCount =
                    shape.mesh.num_face_vertices[face];

                for (int v = 0; v < faceVertexCount; v++)
                {
                    tinyobj::index_t index =
                        shape.mesh.indices[indexOffset + v];

                    Vertex vertex{};

                    vertex.Position =
                    {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                    };

                    vertex.Color = { 1.0f, 1.0f, 1.0f };

                    if (index.texcoord_index >= 0)
                    {
                        vertex.TexCoord =
                        {
                            attrib.texcoords[2 * index.texcoord_index + 0],
                            1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                        };
                    }
                    else
                    {
                        vertex.TexCoord = { 0.0f, 0.0f };
                    }

                    if (uniqueVertices.count(vertex) == 0)
                    {
                        uniqueVertices[vertex] =
                            static_cast<uint32_t>(subMesh.Vertices.size());

                        subMesh.Vertices.push_back(vertex);
                    }

                    subMesh.Indices.push_back(uniqueVertices[vertex]);
                }

                indexOffset += faceVertexCount;
            }
        }

        for (auto& [materialId, subMesh] : subMeshesByMaterial)
        {
            if (materialId >= 0 &&
                materialId < static_cast<int>(materials.size()))
            {
                const auto& material = materials[materialId];

                subMesh.MaterialName = material.name;

                if (!material.diffuse_texname.empty())
                {
                    subMesh.DiffuseTexturePath =
                        parentPath / material.diffuse_texname;
                }
            }
            else
            {
                subMesh.MaterialName = "Default";
            }

            meshData.SubMeshes.push_back(std::move(subMesh));
        }

        return meshData;
    }
}