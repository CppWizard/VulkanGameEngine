#pragma once

#include <Engine/Renderer/Mesh.hpp>
#include <Engine/Renderer/Vulkan/VertexBuffer.hpp>
#include <Engine/Renderer/Vulkan/IndexBuffer.hpp>

#include <memory>
#include <vector>

namespace Engine
{
	class Mesh
	{
	public:
		Mesh(
			VulkanContext& context,
			const std::vector<Vertex>& vertices,
			const std::vector<uint32_t>& indices
		);

		const VertexBuffer& GetVertexBuffer() const;
		const IndexBuffer& GetIndexBuffer() const;

		uint32_t GetIndexCount() const;

	private:
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	};
}