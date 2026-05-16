#include <Engine/Core/Input.hpp>

#include <GLFW/glfw3.h>

namespace Engine
{
	bool Input::IsKeyPressed(const Window& window, int key)
	{
		const auto state = glfwGetKey(window.NativeHandle(), key);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(const Window& window, int button)
	{
		const auto state = glfwGetMouseButton(window.NativeHandle(), button);
		return state == GLFW_PRESS;
	}

	void Input::GetMousePosition(const Window& window, double& x, double& y)
	{
		glfwGetCursorPos(window.NativeHandle(), &x, &y);
	}
}