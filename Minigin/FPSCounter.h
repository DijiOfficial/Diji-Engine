#pragma once
#include "Component.h"
#include <chrono>

class FPSCounter final : public Component
{
public:
	FPSCounter() : m_FrameCount(0), m_Fps(0) {};
	~FPSCounter() = default;

	FPSCounter(const FPSCounter& other) = default;
	FPSCounter(FPSCounter&& other) = default;
	FPSCounter& operator=(const FPSCounter& other) = delete;
	FPSCounter& operator=(FPSCounter&& other) = delete;

	void Update(GameObject& gameObject) override;
	void Render() const override {};
	float GetFPS() const { return m_Fps; };

private:
	const int REFRESH_RATE = 100;
	int m_FrameCount;
	float m_Fps;
	std::chrono::steady_clock::time_point m_LastTime;
	std::chrono::steady_clock::time_point m_LastFpsUpdate;

	void CalculateFps(const std::chrono::steady_clock::time_point& currentTime);
};

