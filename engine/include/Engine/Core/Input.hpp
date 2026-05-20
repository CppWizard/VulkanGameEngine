#pragma once

#include <Engine/Platform/Window.hpp>

#include <array>

namespace Engine
{
	class Input
	{
	public:
		static void Init(Window& window);
		static void Update();
		static void OnEvent(Event& event);

		static bool IsKeyDown(int key);
		static bool IsKeyPressed(int key);
		static bool IsKeyReleased(int key);

		static bool IsMouseButtonDown(int button);
		static bool IsMouseButtonPressed(int button);
		static bool IsMouseButtonReleased(int button);

		static double GetMouseX();
		static double GetMouseY();

		static double GetMouseDeltaX();
		static double GetMouseDeltaY();

		static double GetScrollX();
		static double GetScrollY();

	private:
		static inline GLFWwindow* s_Window = nullptr;

		static inline std::array<bool, 512> s_KeysDown{};
		static inline std::array<bool, 512> s_KeysPressed{};
		static inline std::array<bool, 512> s_KeysReleased{};

		static inline std::array<bool, 16> s_MouseButtonsDown{};
		static inline std::array<bool, 16> s_MouseButtonsPressed{};
		static inline std::array<bool, 16> s_MouseButtonsReleased{};

		static inline double s_MouseX = 0.0;
		static inline double s_MouseY = 0.0;
		static inline double s_LastMouseX = 0.0;
		static inline double s_LastMouseY = 0.0;
		static inline double s_MouseDeltaX = 0.0;
		static inline double s_MouseDeltaY = 0.0;

		static inline double s_ScrollX = 0.0;
		static inline double s_ScrollY = 0.0;
	};
}