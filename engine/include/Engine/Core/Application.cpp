#include <Engine/Core/Application.hpp>
#include <Engine/Core/Logger.hpp>

namespace Engine
{
	Application::Application(const ApplicationSpecification& spec)
		: m_Specification(spec)
	{
		WindowCreateInfo windowInfo;
		windowInfo.Title = spec.Title;
		windowInfo.Width = spec.Width;
		windowInfo.Height = spec.Height;
		windowInfo.Resizable = spec.Resizable;

		m_Window = std::make_unique<Window>(windowInfo);

		ENGINE_INFO("Application created.");
	}

	Application::~Application()
	{
		ENGINE_INFO("Application destroyed.");
	}

	void Application::Run()
	{
		m_Running = true;

		OnStart();

		while (m_Running && !m_Window->ShouldClose())
		{
			const float deltaTime = m_Timer.Tick();

			m_Window->PollEvents();

			OnUpdate(deltaTime);
			OnRender();
		}

		OnShutdown();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	Window& Application::GetWindow()
	{
		return *m_Window;
	}

	const Window& Application::GetWindow() const
	{
		return *m_Window;
	}
}