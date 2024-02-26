#include "FPSCounter.h"
#include "Text.h"
#include <format>

void diji::FPSCounter::Update()
{
    if (not m_TextComponentPtr)
        m_TextComponentPtr = GetOwner()->GetComponent<Text>();

    ++m_FrameCount;
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_LastTime).count();

    if (elapsedTime >= REFRESH_RATE)
    {
        CalculateFps(currentTime);
        m_FrameCount = 0;
        m_LastTime = currentTime;
    }

    //check if fps changed
    m_TextComponentPtr->SetText(std::format("{:.1f} FPS", m_Fps));
}

void diji::FPSCounter::CalculateFps(const std::chrono::steady_clock::time_point& currentTime)
{
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_LastFpsUpdate).count();
    m_Fps = static_cast<double>((m_FrameCount * 1000.f) / elapsedTime);
    m_LastFpsUpdate = currentTime;
};