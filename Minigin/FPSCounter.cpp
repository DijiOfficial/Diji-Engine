#include "FPSCounter.h"
#include "Text.h"
#include "TimeSingleton.h"

#include <format>

diji::FPSCounter::FPSCounter(GameObject* ownerPtr) 
    : Component(ownerPtr)
    , m_FrameCount{ 0 }
    , m_Fps{ 0 }
    , m_ElapsedTime{ 0 } 
{ 
    m_TextComponentPtr = ownerPtr->GetComponent<Text>(); 

    assert(m_TextComponentPtr and "Text Component needs to be initialized before FPSCounter");
};


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