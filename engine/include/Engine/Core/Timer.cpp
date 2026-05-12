#include <Engine/Core/Timer.hpp>

namespace Engine
{
	Timer::Timer()
	{
		Reset();
	}

	void Timer::Reset()
	{
		m_LastTime = Clock::now();
		m_DeltaTime = 0.0f;
	}

	float Timer::Tick()
	{
		const auto currentTime = Clock::now();

		m_DeltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count();

		m_LastTime = currentTime;
		return m_DeltaTime;
	}

	float Timer::DeltaTime() const
	{
		return m_DeltaTime;
	}
}