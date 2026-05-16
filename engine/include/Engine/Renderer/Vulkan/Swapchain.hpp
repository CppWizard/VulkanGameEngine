#pragma once

#include <Engine/Renderer/Vulkan/VulkanContext.hpp>
#include <Engine/Platform/Window.hpp>

#include <vulkan/vulkan.hpp>

#include <vector>
#include <memory>

namespace Engine
{
	struct SwapchainSupportDetails
	{
		VkSurfaceCapabilitiesKHR Capabilities{};
		std::vector<VkSurfaceFormatKHR> Formats;
		std::vector<VkPresentModeKHR> PresentModes;
	};

	class Swapchain
	{
	public:
		Swapchain(const VulkanContext& context, const Window& window);
		~Swapchain();

		Swapchain(const Swapchain&) = delete;
		Swapchain& operator=(const Swapchain&) = delete;

		void Recreate(const Window& window);

		VkSwapchainKHR GetHandle() const { return m_Swapchain; }
		VkFormat GetImageFormat() const { return m_ImageFormat; }
		VkExtent2D GetExtent() const { return m_Extent; }

		const std::vector<VkImage>& GetImages() const { return m_Images; }
		const std::vector<VkImageView>& GetImageViews() const { return m_ImageViews; }

		uint32_t GetImageCount() const
		{
			return static_cast<uint32_t>(m_Images.size());
		}

	private:
		void Create(const Window& window);
		void Cleanup();

		SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device) const;

		VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) const;

		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes) const;

		VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities, const Window& window) const;

		void CreateImageViews();

	private:
		const VulkanContext& m_Context;

		VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;

		std::vector<VkImage> m_Images;
		std::vector<VkImageView> m_ImageViews;

		VkFormat m_ImageFormat;
		VkExtent2D m_Extent;
	};
}