#include <Engine/Renderer/MeshFactory.hpp>
#include <Engine/Assets/ObjLoader.hpp>

std::shared_ptr<Engine::Mesh> Engine::MeshFactory::LoadObj(
    VulkanContext& context,
    const CommandPool& commandPool,
	const DescriptorPool& descriptorPool,
	const MaterialDescriptorSetLayout& materialDescriptorSetLayout,
    const std::filesystem::path& path,
    const std::filesystem::path& fallbackTexture
)
{
    ObjMeshData data = ObjLoader::Load(path);

    auto mesh = std::make_shared<Mesh>();

    for (const auto& subMeshData : data.SubMeshes)
    {
        auto material = std::make_shared<Material>();
        material->Name = subMeshData.MaterialName;

        std::filesystem::path texturePath =
            subMeshData.DiffuseTexturePath.empty()
            ? fallbackTexture
            : subMeshData.DiffuseTexturePath;

        material->DiffuseTexture =
            std::make_shared<Texture2D>(
                context,
                commandPool,
                texturePath
            );

        material->DescriptorSet =
            std::make_unique<MaterialDescriptorSet>(
                context,
                descriptorPool.GetHandle(),
                materialDescriptorSetLayout,
                *material->DiffuseTexture
            );

        auto subMesh = std::make_unique<SubMesh>();

        subMesh->Vertices =
            std::make_unique<VertexBuffer>(
                context,
                subMeshData.Vertices
            );

        subMesh->Indices =
            std::make_unique<IndexBuffer>(
                context,
                subMeshData.Indices
            );

        subMesh->MaterialData = material;

        mesh->AddSubMesh(std::move(subMesh));
    }

    return mesh;
}