#pragma once

#include <Engine/Renderer/Vulkan/Buffer.hpp>
#include <Engine/Renderer/Vertex.hpp>

#include <memory>
#include <vector>

namespace Engine
{
    class VertexBuffer
    {
    public:
        VertexBuffer(
            VulkanContext& context,
            const std::vector<Vertex>& vertices
        );

        VkBuffer GetHandle() const
        {
            return m_Buffer->GetHandle();
        }

        uint32_t GetVertexCount() const
        {
            return m_VertexCount;
        }

    private:
        std::unique_ptr<Buffer> m_Buffer;
        uint32_t m_VertexCount = 0;
    };
}