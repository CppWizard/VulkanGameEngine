#pragma once

namespace Engine
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	protected:
		virtual void OnStart() {}
		virtual void OnUpdate() {}
		virtual void OnShutdown() {}
	};

}