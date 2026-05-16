#pragma once

#include <Engine/Platform/Window.hpp>

#include <vulkan/vulkan.hpp>

#include <vector>
#include <optional>

namespace Engine
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> GraphicsFamily;
		std::optional<uint32_t> PresentFamily;

		bool isComplete() const
		{
			return GraphicsFamily.has_value() && PresentFamily.has_value();
		}
	};

	class VulkanContext
	{
	public:
		explicit VulkanContext(const Window& window);
		~VulkanContext();

		//VulkanContext(const VulkanContext&) = delete;
		VulkanContext& operator=(const VulkanContext&) = delete;

		VkInstance getInstance() const { return m_Instance; }
		VkPhysicalDevice getPhysicalDevice() const { return m_PhysicalDevice; }
		VkDevice getDevice() const { return m_Device; }
		VkSurfaceKHR getSurface() const { return m_Surface; }

		VkQueue getGraphicsQueue() const { return m_GraphicsQueue; }
		VkQueue getPresentQueue() const { return m_PresentQueue; }

		QueueFamilyIndices GetQueueFamilies() const;

	private:
		void Init(const Window& window);
		void Shutdown();

		void CreateInstance();
		void SetupDebugMessenger();
		void CreateSurface(const Window& window);
		void PickPhysicalDevice();
		void CreateLogicalDevice();

		bool CheckValidationLayerSupport() const;
		std::vector<const char*> GetRequiredExtensions() const;

		bool IsDeviceSuitable(VkPhysicalDevice device) const;
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;

	private:
		VkInstance m_Instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;
		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device = VK_NULL_HANDLE;

		VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
		VkQueue m_PresentQueue = VK_NULL_HANDLE;

		const std::vector<const char*> m_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

#ifdef ENGINE_DEBUG
		const bool m_EnableValidationLayers = true;
#else
		const bool m_EnableValidationLayers = false;
#endif
	};

}