#include <Engine/Platform/Window.hpp>
#include <Engine/Core/Logger.hpp>

#include <GLFW/glfw3.h>

#include <stdexcept>

namespace Engine
{
	Window::Window(const WindowCreateInfo& info)
	{
		Init(info);
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Init(const WindowCreateInfo& info)
	{
		m_Width = info.Width;
		m_Height = info.Height;

		if (!glfwInit())
		{
			throw std::runtime_error("Failed to initialize GLFW.");
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, info.Resizable ? GLFW_TRUE : GLFW_FALSE);

		m_Window = glfwCreateWindow(
			static_cast<int>(info.Width),
			static_cast<int>(info.Height),
			info.Title.c_str(),
			nullptr,
			nullptr
		);

		if (!m_Window)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window.");
		}

		glfwSetWindowUserPointer(m_Window, this);

		glfwSetFramebufferSizeCallback(
			m_Window,
			[](GLFWwindow* window, int width, int height)
			{
				auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

				self->m_Width = static_cast<uint32_t>(width);
				self->m_Height = static_cast<uint32_t>(height);
				self->m_FramebufferResized = true;
			}
		);

		ENGINE_INFO("Window created.");
	}

	void Window::Shutdown()
	{
		if (m_Window)
		{
			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
		}

		glfwTerminate();

		ENGINE_INFO("Window destroyed.");
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	uint32_t Window::Width() const
	{
		return m_Width;
	}

	uint32_t Window::Height() const
	{
		return m_Height;
	}

	GLFWwindow* Window::NativeHandle() const
	{
		return m_Window;
	}

	bool Window::WasResized() const
	{
		return m_FramebufferResized;
	}

	void Window::ResetResizeFlag()
	{
		m_FramebufferResized = false;
	}
}