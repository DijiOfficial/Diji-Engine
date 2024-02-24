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

	//void Update(std::vector<std::unique_ptr<Component>>& compVec) override;
	void Update() override;
	void Render() const override {};
	int GetFPS() const { return m_Fps; };

private:
	int m_FrameCount;
	int m_Fps;
	std::chrono::steady_clock::time_point m_LastTime;
};

