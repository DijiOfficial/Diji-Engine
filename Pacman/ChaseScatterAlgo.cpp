#include "ChaseScatterAlgo.h"
#include "TimeSingleton.h"

void pacman::ChaseScatterAlgo::Update()
{
	if (m_CurrentCycle > m_ChaseScatterDuration.size() - 1)
		return;

	m_TotalElapsedTime += diji::TimeSingleton::GetInstance().GetDeltaTime();
	if (m_TotalElapsedTime >= m_ChaseScatterDuration[m_CurrentCycle])
	{
		m_TotalElapsedTime -= m_ChaseScatterDuration[m_CurrentCycle];
		m_IsInChaseState = !m_IsInChaseState;
		++m_CurrentCycle;
	}
}
