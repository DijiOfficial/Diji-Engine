#include "FPSCounter.h"
#include <iostream>

void FPSCounter::Update(GameObject& gameObject)
{
    (void)gameObject; // Marking the parameter as unused

    ++m_FrameCount;
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_LastTime).count();

    if (elapsedTime >= REFRESH_RATE)
    {
        CalculateFps(currentTime);
        m_FrameCount = 0;
        m_LastTime = currentTime;
    }
}

void FPSCounter::CalculateFps(const std::chrono::steady_clock::time_point& currentTime)
{
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_LastFpsUpdate).count();
    m_Fps = static_cast<float>((m_FrameCount * 1000.f) / elapsedTime);
    m_LastFpsUpdate = currentTime;
};