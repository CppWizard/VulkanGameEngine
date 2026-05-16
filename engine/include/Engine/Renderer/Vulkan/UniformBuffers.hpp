#pragma once

#include <Engine/Renderer/Vulkan/Buffer.hpp>
#include <Engine/Renderer/Vulkan/SyncObjects.hpp>
#include <Engine/Renderer/UniformBufferObject.hpp>

#include <memory>
#include <vector>

namespace Engine
{
    class UniformBuffers
    {
    public:
        explicit UniformBuffers(VulkanContext& context);

        void Update(uint32_t frameIndex, const UniformBufferObject& ubo);

        VkBuffer GetBuffer(uint32_t frameIndex) const;
        VkDeviceSize GetBufferSize() const;

    private:
        std::vector<std::unique_ptr<Buffer>> m_Buffers;
    };
}