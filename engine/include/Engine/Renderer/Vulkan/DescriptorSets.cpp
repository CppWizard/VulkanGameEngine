#include <Engine/Renderer/Vulkan/DescriptorSets.hpp>
#include <stdexcept>
#include <array>

namespace Engine
{
    DescriptorSets::DescriptorSets(
        VulkanContext& context,
        const DescriptorPool& pool,
        const DescriptorSetLayout& layout,
        const UniformBuffers& uniformBuffers
    )
        : m_Context(context)
    {
        std::vector<VkDescriptorSetLayout> layouts(
            SyncObjects::MaxFramesInFlight,
            layout.GetHandle()
        );


        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = pool.GetHandle();
        allocInfo.descriptorSetCount = SyncObjects::MaxFramesInFlight;
        allocInfo.pSetLayouts = layouts.data();

        m_DescriptorSets.resize(SyncObjects::MaxFramesInFlight);

        if (vkAllocateDescriptorSets(
            m_Context.getDevice(),
            &allocInfo,
            m_DescriptorSets.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate descriptor sets.");
        }

        for (uint32_t i = 0; i < SyncObjects::MaxFramesInFlight; i++)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers.GetBuffer(i);
            bufferInfo.offset = 0;
            bufferInfo.range = uniformBuffers.GetBufferSize();

            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = m_DescriptorSets[i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;

            vkUpdateDescriptorSets(
                m_Context.getDevice(),
                1,
                &descriptorWrite,
                0,
                nullptr
            );
        }
    }
}