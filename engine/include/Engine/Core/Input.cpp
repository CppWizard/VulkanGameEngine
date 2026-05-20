#include <Engine/Core/Input.hpp>
#include <Engine/Core/EventDispatcher.hpp>

namespace Engine
{
	void Input::Init(Window& window)
	{
		s_KeysDown.fill(false);
		s_KeysPressed.fill(false);
		s_KeysReleased.fill(false);

		s_MouseButtonsDown.fill(false);
		s_MouseButtonsPressed.fill(false);
		s_MouseButtonsReleased.fill(false);

		s_Window = window.NativeHandle();

		s_LastMouseX = s_MouseX;
		s_LastMouseY = s_MouseY;
	}

	void Input::Update()
	{
		s_KeysPressed.fill(false);
		s_KeysReleased.fill(false);

		s_MouseButtonsPressed.fill(false);
		s_MouseButtonsReleased.fill(false);

		s_MouseDeltaX = 0.0;
		s_MouseDeltaY = 0.0;

		s_ScrollX = 0.0;
		s_ScrollY = 0.0;

		s_LastMouseX = s_MouseX;
		s_LastMouseY = s_MouseY;
	}

	void Input::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<KeyPressedEvent>(
			[](KeyPressedEvent& e)
			{
				if (e.Key >= 0 && e.Key < static_cast<int>(s_KeysDown.size()))
				{
					if (!e.Repeat)
					{
						s_KeysPressed[e.Key] = true;
					}

					s_KeysDown[e.Key] = true;
				}

				return false;
			}
		);

		dispatcher.Dispatch<KeyReleasedEvent>(
			[](KeyReleasedEvent& e)
			{
				if (e.Key >= 0 && e.Key < static_cast<int>(s_KeysDown.size()))
				{
					s_KeysDown[e.Key] = false;
					s_KeysReleased[e.Key] = true;
				}

				return false;
			}
		);

		dispatcher.Dispatch<MouseButtonPressedEvent>(
			[](MouseButtonPressedEvent& e)
			{
				if (e.Button >= 0 &&
					e.Button < static_cast<int>(s_MouseButtonsDown.size()))
				{
					s_MouseButtonsDown[e.Button] = true;
					s_MouseButtonsPressed[e.Button] = true;
				}

				return false;
			}
		);

		dispatcher.Dispatch<MouseButtonReleasedEvent>(
			[](MouseButtonReleasedEvent& e)
			{
				if (e.Button >= 0 &&
					e.Button < static_cast<int>(s_MouseButtonsDown.size()))
				{
					s_MouseButtonsDown[e.Button] = false;
					s_MouseButtonsReleased[e.Button] = true;
				}

				return false;
			}
		);

		dispatcher.Dispatch<MouseMovedEvent>(
			[](MouseMovedEvent& e)
			{
				s_MouseDeltaX += e.X - s_MouseX;
				s_MouseDeltaY += e.Y - s_MouseY;

				s_MouseX = e.X;
				s_MouseY = e.Y;

				return false;
			}
		);

		dispatcher.Dispatch<MouseScrolledEvent>(
			[](MouseScrolledEvent& e)
			{
				s_ScrollX += e.XOffset;
				s_ScrollY += e.YOffset;

				return false;
			}
		);
	}

	bool Input::IsKeyDown(int key)
	{
		return key >= 0 &&
			key < static_cast<int>(s_KeysDown.size()) &&
			s_KeysDown[key];
	}

	bool Input::IsKeyPressed(int key)
	{
		return key >= 0 &&
			key < static_cast<int>(s_KeysPressed.size()) &&
			s_KeysPressed[key];
	}

	bool Input::IsKeyReleased(int key)
	{
		return key >= 0 &&
			key < static_cast<int>(s_KeysReleased.size()) &&
			s_KeysReleased[key];
	}

	bool Input::IsMouseButtonDown(int button)
	{
		return button >= 0 &&
			button < static_cast<int>(s_MouseButtonsDown.size()) &&
			s_MouseButtonsDown[button];
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		return button >= 0 &&
			button < static_cast<int>(s_MouseButtonsPressed.size()) &&
			s_MouseButtonsPressed[button];
	}

	bool Input::IsMouseButtonReleased(int button)
	{
		return button >= 0 &&
			button < static_cast<int>(s_MouseButtonsReleased.size()) &&
			s_MouseButtonsReleased[button];
	}

	double Input::GetMouseX()
	{
		return s_MouseX;
	}

	double Input::GetMouseY()
	{
		return s_MouseY;
	}

	double Input::GetMouseDeltaX()
	{
		return s_MouseDeltaX;
	}

	double Input::GetMouseDeltaY()
	{
		return s_MouseDeltaY;
	}

	double Input::GetScrollX()
	{
		return s_ScrollX;
	}

	double Input::GetScrollY()
	{
		return s_ScrollY;
	}
}