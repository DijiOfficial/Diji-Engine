#include "FPSCounter.h"
#include "Text.h"
#include "TimeSingleton.h"
#include "GameObject.h"
#include <format>

diji::FPSCounter::FPSCounter(GameObject* ownerPtr) 
    : Component(ownerPtr)
    , m_TextComponentPtr{ nullptr }
    , m_FrameCount{ 0 }
    , m_Fps{ 0 }
    , m_ElapsedTime{ 0 } 
{ 
};

void diji::FPSCounter::Init()
{
    m_TextComponentPtr = GetOwner()->GetComponent<Text>();
}

void diji::FPSCounter::Update()
{
    const double tempFps = m_Fps;
    m_ElapsedTime += TimeSingleton::GetInstance().GetDeltaTime();
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
