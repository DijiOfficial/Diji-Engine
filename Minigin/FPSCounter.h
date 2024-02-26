#pragma once
#include "Component.h"
#include <chrono>

namespace diji
{
	class FPSCounter final : public Component
	{
	public:
		FPSCounter(GameObject* ownerPtr) : Component(ownerPtr), m_FrameCount{ 0 }, m_Fps{ 0 } { m_TextComponentPtr = GetOwner()->GetComponent<Text>(); };
		~FPSCounter() = default;

		FPSCounter(const FPSCounter& other) = delete;
		FPSCounter(FPSCounter&& other) = delete;
		FPSCounter& operator=(const FPSCounter& other) = delete;
		FPSCounter& operator=(FPSCounter&& other) = delete;

		void Update() override;
		double GetFPS() const { return m_Fps; };

	private:
		const int REFRESH_RATE = 100;
		int m_FrameCount;
		double m_Fps;
		std::chrono::steady_clock::time_point m_LastTime;
		std::chrono::steady_clock::time_point m_LastFpsUpdate;
		Text* m_TextComponentPtr;

		void CalculateFps(const std::chrono::steady_clock::time_point& currentTime);
	};
}

