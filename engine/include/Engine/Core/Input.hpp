#pragma once

#include <Engine/Platform/Window.hpp>

namespace Engine
{
	class Input
	{
	public:
		static bool IsKeyPressed(const Window& window, int key);
		static bool IsMouseButtonPressed(const Window& window, int button);

		static void GetMousePosition(const Window& window, double& x, double& y);
	};
}