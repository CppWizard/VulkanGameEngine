#include <Engine/Renderer/Vulkan/DescriptorSetLayout.hpp>
#include <stdexcept>
#include <array>

namespace Engine
{
    DescriptorSetLayout::DescriptorSetLayout(VulkanContext& context)
        : m_Context(context)
    {
        VkDescriptorSetLayoutBinding uboBinding{};
        uboBinding.binding = 0;
        uboBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboBinding.descriptorCount = 1;
        uboBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding samplerBinding{};
        samplerBinding.binding = 1;
        samplerBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerBinding.descriptorCount = 1;
        samplerBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings =
        {
            uboBinding,
            samplerBinding
        };

        VkDescriptorSetLayoutCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        info.bindingCount = static_cast<uint32_t>(bindings.size());
        info.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(
            m_Context.getDevice(),
            &info,
            nullptr,
            &m_Layout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create descriptor set layout.");
        }
    }

    DescriptorSetLayout::~DescriptorSetLayout()
    {
        if (m_Layout)
        {
            vkDestroyDescriptorSetLayout(
                m_Context.getDevice(),
                m_Layout,
                nullptr
            );
        }
    }
}