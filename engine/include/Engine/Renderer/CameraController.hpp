#pragma once

#include <Engine/Renderer/Camera.hpp>

namespace Engine
{
	class CameraController
	{
	public:
		explicit CameraController(Camera& camera);

		void Update(float deltaTime);

		void SetMoveSpeed(float speed) { m_MoveSpeed = speed; }
		void SetMouseSensitivity(float sensitivity) { m_MouseSensitivity = sensitivity; }

	private:
		Camera& m_Camera;

		float m_MoveSpeed = 3.0f;
		float m_MouseSensitivity = 0.1f;

		float m_Yaw = -90.0f; // Initialized to look along the negative Z-axis
		float m_Pitch = 0.0f;

		bool m_FirstMouse = true;
	};
}