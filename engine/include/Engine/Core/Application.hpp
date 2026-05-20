#pragma once

#include <Engine/Platform/Window.hpp>
#include <Engine/Core/Timer.hpp>
#include <Engine/Core/Event.hpp>

#include <memory>
#include <deque>
#include <string>
#include <cstdint>

namespace Engine
{
	struct ApplicationSpecification
	{
		std::string Title = "Vulkan Game Engine";
		uint32_t Width = 1280;
		uint32_t Height = 720;
		bool Resizable = true;
	};

	class Application
	{
	public:
		explicit Application(const ApplicationSpecification& spec = {});
		virtual ~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void Run();
		void Close();

		Window& GetWindow();
		const Window& GetWindow() const;

		void OnEvent(Event& event);

	protected:
		virtual void OnStart() {}
		virtual void OnUpdate(float) {}
		virtual void OnRender() {}
		virtual void OnShutdown() {}
		virtual void OnAppEvent(Event& event) {}
		const std::deque<std::string>& GetEventLog() const { return m_EventLog; }

	private:
		ApplicationSpecification m_Specification;
		std::unique_ptr<Window> m_Window;
		std::deque<std::string> m_EventLog;
		Timer m_Timer;
		bool m_Running = false;
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);
	};
}