#include "FPSCounter.h"
#include "Text.h"
#include <format>

void diji::FPSCounter::Update()
{
    if (not m_TextComponentPtr) //assuming GameObject has a Text component
        m_TextComponentPtr = GetOwner()->GetComponent<Text>();

    const double tempFps = m_Fps;
    m_ElapsedTime += Time::GetInstance().GetDeltaTime();
    ++m_FrameCount;

    if (m_ElapsedTime >= REFRESH_RATE)
    {
        m_Fps = static_cast<float>(m_FrameCount / m_ElapsedTime);
        m_FrameCount = 0;
        m_ElapsedTime = 0;
    }

    if (m_Fps != tempFps)
        m_TextComponentPtr->SetText(std::format("{:.1f} FPS", m_Fps));
}