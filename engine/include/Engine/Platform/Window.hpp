#pragma once

#include <string>
#include <cstdint>

struct GLFWwindow;

namespace Engine
{
	struct WindowCreateInfo
	{
		std::string Title = "Vulkan Game Engine";
		uint32_t Width = 1280;
		uint32_t Height = 720;
		bool Resizable = true;
	};

	class Window
	{
	public:
		explicit Window(const WindowCreateInfo& info);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		bool ShouldClose() const;
		void PollEvents();

		uint32_t Width() const;
		uint32_t Height() const;

		bool WasResized() const;
		void ResetResizeFlag();

		GLFWwindow* NativeHandle() const;

	private:
		void Init(const WindowCreateInfo& info);
		void Shutdown();

	private:
		GLFWwindow* m_Window = nullptr;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		bool m_FramebufferResized = false;
	};
}