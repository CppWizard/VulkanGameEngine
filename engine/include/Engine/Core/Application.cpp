#include <Engine/Core/Application.hpp>
#include <Engine/Core/Logger.hpp>
#include <Engine/Core/Input.hpp>
#include <Engine/Core/EventDispatcher.hpp>

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

		Input::Init(*m_Window);

		m_Window->SetEventCallback(
			[this](Event& event)
			{
				OnEvent(event);
			}
		);

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

			Input::Update();
			m_Window->PollEvents();

			m_Window->SetEventCallback(
				[this](Event& event)
				{
					OnEvent(event);
				}
			);

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

	void Application::OnEvent(Event& event)
	{
		m_EventLog.push_front(event.ToString());

		if (m_EventLog.size() > 100)
		{
			m_EventLog.pop_back();
		}

		Input::OnEvent(event);

		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(
			[this](WindowCloseEvent& e)
			{
				return OnWindowClose(e);
			}
		);

		dispatcher.Dispatch<WindowResizeEvent>(
			[this](WindowResizeEvent& e)
			{
				return OnWindowResize(e);
			}
		);

		OnAppEvent(event);
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		(void)event;
		Close();
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		(void)event;
		return false;
	}
}