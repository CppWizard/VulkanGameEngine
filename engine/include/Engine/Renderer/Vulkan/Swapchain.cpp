#include <Engine/Renderer/Vulkan/Swapchain.hpp>
#include <Engine/Core/Logger.hpp>

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace Engine
{
	Swapchain::Swapchain(const VulkanContext& context, const Window& window)
		: m_Context(context)
	{
		Create(window);
	}

	Swapchain::~Swapchain()
	{
		Cleanup();
	}

	void Swapchain::Recreate(const Window& window)
	{
		vkDeviceWaitIdle(m_Context.getDevice());

		Cleanup();
		Create(window);
	}

	void Swapchain::Create(const Window& window)
	{
		SwapchainSupportDetails swapchainSupport =
			QuerySwapchainSupport(m_Context.getPhysicalDevice());

		VkSurfaceFormatKHR surfaceFormat =
			ChooseSurfaceFormat(swapchainSupport.Formats);

		VkPresentModeKHR presentMode =
			ChoosePresentMode(swapchainSupport.PresentModes);

		VkExtent2D extent =
			ChooseExtent(swapchainSupport.Capabilities, window);

		uint32_t imageCount = swapchainSupport.Capabilities.minImageCount + 1;

		if (swapchainSupport.Capabilities.maxImageCount > 0 &&
			imageCount > swapchainSupport.Capabilities.maxImageCount)
		{
			imageCount = swapchainSupport.Capabilities.maxImageCount;
		}

		QueueFamilyIndices indices = m_Context.GetQueueFamilies();

		uint32_t queueFamilyIndices[] =
		{
			indices.GraphicsFamily.value(),
			indices.PresentFamily.value()
		};

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Context.getSurface();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		if (indices.GraphicsFamily != indices.PresentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = swapchainSupport.Capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(
			m_Context.getDevice(),
			&createInfo,
			nullptr,
			&m_Swapchain) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create swapchain.");
		}

		vkGetSwapchainImagesKHR(
			m_Context.getDevice(),
			m_Swapchain,
			&imageCount,
			nullptr
		);

		m_Images.resize(imageCount);

		vkGetSwapchainImagesKHR(
			m_Context.getDevice(),
			m_Swapchain,
			&imageCount,
			m_Images.data()
		);

		m_ImageFormat = surfaceFormat.format;
		m_Extent = extent;

		CreateImageViews();

		ENGINE_INFO("Swapchain created.");
	}

	void Swapchain::Cleanup()
	{
		VkDevice device = m_Context.getDevice();

		for (VkImageView imageView : m_ImageViews)
		{
			vkDestroyImageView(device, imageView, nullptr);
		}

		m_ImageViews.clear();
		m_Images.clear();

		if (m_Swapchain)
		{
			vkDestroySwapchainKHR(device, m_Swapchain, nullptr);
			m_Swapchain = VK_NULL_HANDLE;
		}

		ENGINE_INFO("Swapchain destroyed.");
	}

	SwapchainSupportDetails Swapchain::QuerySwapchainSupport(
		VkPhysicalDevice device) const
	{
		SwapchainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
			device,
			m_Context.getSurface(),
			&details.Capabilities
		);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(
			device,
			m_Context.getSurface(),
			&formatCount,
			nullptr
		);

		if (formatCount != 0)
		{
			details.Formats.resize(formatCount);

			vkGetPhysicalDeviceSurfaceFormatsKHR(
				device,
				m_Context.getSurface(),
				&formatCount,
				details.Formats.data()
			);
		}

		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(
			device,
			m_Context.getSurface(),
			&presentModeCount,
			nullptr
		);

		if (presentModeCount != 0)
		{
			details.PresentModes.resize(presentModeCount);

			vkGetPhysicalDeviceSurfacePresentModesKHR(
				device,
				m_Context.getSurface(),
				&presentModeCount,
				details.PresentModes.data()
			);
		}

		return details;
	}

	VkSurfaceFormatKHR Swapchain::ChooseSurfaceFormat(
		const std::vector<VkSurfaceFormatKHR>& formats) const
	{
		for (const auto& availableFormat : formats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
				availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return formats[0];
	}

	VkPresentModeKHR Swapchain::ChoosePresentMode(
		const std::vector<VkPresentModeKHR>& presentModes) const
	{
		for (const auto& availablePresentMode : presentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D Swapchain::ChooseExtent(
		const VkSurfaceCapabilitiesKHR& capabilities,
		const Window& window) const
	{
		if (capabilities.currentExtent.width !=
			std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}

		VkExtent2D actualExtent =
		{
			window.Width(),
			window.Height()
		};

		actualExtent.width = std::clamp(
			actualExtent.width,
			capabilities.minImageExtent.width,
			capabilities.maxImageExtent.width
		);

		actualExtent.height = std::clamp(
			actualExtent.height,
			capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height
		);

		return actualExtent;
	}

	void Swapchain::CreateImageViews()
	{
		m_ImageViews.resize(m_Images.size());

		for (size_t i = 0; i < m_Images.size(); i++)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_Images[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_ImageFormat;

			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			createInfo.subresourceRange.aspectMask =
				VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;
			if (vkCreateImageView(
				m_Context.getDevice(),
				&createInfo,
				nullptr,
				&m_ImageViews[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create image views.");
			}
		}
	}
}