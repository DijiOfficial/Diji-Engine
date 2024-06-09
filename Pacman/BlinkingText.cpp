#include "BlinkingText.h"
#include "Render.h"
#include "GameObject.h"
#include "TimeSingleton.h"
void pacman::BlinkingText::Init()
{
	m_RenderCompPtr = GetOwner()->GetComponent<diji::Render>();
}

void pacman::BlinkingText::Update()
{
	constexpr float m_BlinkInterval = 0.5f;
	m_BlinkTimer += diji::TimeSingleton::GetInstance().GetDeltaTime();
	if (m_BlinkTimer > m_BlinkInterval)
	{
		m_BlinkTimer = 0;
		m_IsVisible = !m_IsVisible;
		
		if (m_IsVisible)
			m_RenderCompPtr->EnableRender();
		else
			m_RenderCompPtr->DisableRender();
	}
}
