#include <Engine/Renderer/Vulkan/DescriptorSets.hpp>
#include <stdexcept>
#include <array>

namespace Engine
{
    DescriptorSets::DescriptorSets(
        VulkanContext& context,
        const DescriptorPool& pool,
        const DescriptorSetLayout& layout,
        const UniformBuffers& uniformBuffers,
        const Texture2D& texture
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

        if (texture.GetImageView() == VK_NULL_HANDLE ||
            texture.GetSampler() == VK_NULL_HANDLE)
        {
            throw std::runtime_error(
                "Texture image view or sampler is null before descriptor update."
            );
        }

        for (uint32_t i = 0; i < SyncObjects::MaxFramesInFlight; i++)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers.GetBuffer(i);
            bufferInfo.offset = 0;
            bufferInfo.range = uniformBuffers.GetBufferSize();

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = texture.GetImageView();
            imageInfo.sampler = texture.GetSampler();

            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = m_DescriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = m_DescriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(
                m_Context.getDevice(),
                static_cast<uint32_t>(descriptorWrites.size()),
                descriptorWrites.data(),
                0,
                nullptr
            );
        }
    }
}