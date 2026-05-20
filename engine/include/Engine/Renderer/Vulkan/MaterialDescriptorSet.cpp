#include <Engine/Renderer/Vulkan/MaterialDescriptorSet.hpp>
#include <stdexcept>

namespace Engine
{
    MaterialDescriptorSet::MaterialDescriptorSet(
        VulkanContext& context,
        VkDescriptorPool descriptorPool,
        const MaterialDescriptorSetLayout& layout,
        const Texture2D& texture
    )
        : m_Context(context)
    {
        VkDescriptorSetLayout layouts[] =
        {
            layout.GetHandle()
        };

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = layouts;

        if (vkAllocateDescriptorSets(
            m_Context.getDevice(),
            &allocInfo,
            &m_DescriptorSet) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate material descriptor set.");
        }

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = texture.GetImageView();
        imageInfo.sampler = texture.GetSampler();

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = m_DescriptorSet;
        write.dstBinding = 0;
        write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write.descriptorCount = 1;
        write.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(
            m_Context.getDevice(),
            1,
            &write,
            0,
            nullptr
        );
    }
}