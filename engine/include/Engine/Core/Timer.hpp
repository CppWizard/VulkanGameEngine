#pragma once

#include <chrono>

namespace Engine
{
	class Timer
	{
	public:
		Timer();

		void Reset();
		float Tick();

		float DeltaTime() const;

	private:
		using Clock = std::chrono::high_resolution_clock;

		Clock::time_point m_LastTime;
		float m_DeltaTime = 0.0f;
	};
}