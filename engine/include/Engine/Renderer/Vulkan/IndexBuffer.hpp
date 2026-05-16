#pragma once

#include <Engine/Renderer/Vulkan/Buffer.hpp>

#include <cstdint>
#include <memory>
#include <vector>

namespace Engine
{
    class IndexBuffer
    {
    public:
        IndexBuffer(
            VulkanContext& context,
            const std::vector<uint32_t>& indices
        );

        VkBuffer GetHandle() const
        {
            return m_Buffer->GetHandle();
        }

        uint32_t GetIndexCount() const
        {
            return m_IndexCount;
        }

    private:
        std::unique_ptr<Buffer> m_Buffer;
        uint32_t m_IndexCount = 0;
    };
}