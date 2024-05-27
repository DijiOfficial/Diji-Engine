#include "GhostsAlgorithm.h"
#include "TimeSingleton.h"

void pacman::GhostsTimers::Update()
{
	const auto& deltaTime = diji::TimeSingleton::GetInstance().GetDeltaTime();

	if (m_IsPaused)
	{
		m_PauseTimer += deltaTime;
		if (m_PauseTimer >= 2.f)
		{
			m_IsPaused = false;
			m_PauseTimer = 0.f;
		}
	}

	if (m_CurrentCycle > m_ChaseScatterDuration.size() - 1)
		return;

	m_TotalElapsedTime += deltaTime;
	if (m_TotalElapsedTime >= m_ChaseScatterDuration[m_CurrentCycle])
	{
		m_TotalElapsedTime -= m_ChaseScatterDuration[m_CurrentCycle];
		m_IsInChaseState = !m_IsInChaseState;
		++m_CurrentCycle;
	}
}
