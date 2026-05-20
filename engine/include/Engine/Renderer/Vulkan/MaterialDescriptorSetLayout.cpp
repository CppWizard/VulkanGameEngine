#include <Engine/Renderer/Vulkan/MaterialDescriptorSetLayout.hpp>
#include <stdexcept>

namespace Engine
{
    MaterialDescriptorSetLayout::MaterialDescriptorSetLayout(VulkanContext& context)
        : m_Context(context)
    {
        VkDescriptorSetLayoutBinding samplerBinding{};
        samplerBinding.binding = 0;
        samplerBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerBinding.descriptorCount = 1;
        samplerBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        info.bindingCount = 1;
        info.pBindings = &samplerBinding;

        if (vkCreateDescriptorSetLayout(
            m_Context.getDevice(),
            &info,
            nullptr,
            &m_Layout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create material descriptor set layout.");
        }
    }

    MaterialDescriptorSetLayout::~MaterialDescriptorSetLayout()
    {
        if (m_Layout)
        {
            vkDestroyDescriptorSetLayout(m_Context.getDevice(), m_Layout, nullptr);
        }
    }
}