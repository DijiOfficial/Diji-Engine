#include "FPSCounter.h"

void FPSCounter::Update()
{
    ++m_FrameCount;
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_LastTime).count();

    if (elapsedTime >= 1000)
    {
        m_Fps = m_FrameCount;
        m_FrameCount = 0;
        m_LastTime = currentTime;
    }
};