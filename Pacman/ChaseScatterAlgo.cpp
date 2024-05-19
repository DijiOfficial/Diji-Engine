#include "ChaseScatterAlgo.h"
#include "TimeSingleton.h"

void diji::ChaseScatterAlgo::Update()
{
	if (m_CurrentCycle > m_ChaseScatterDuration.size())
		return;

	m_TotalElapsedTime += TimeSingleton::GetInstance().GetDeltaTime();
	if (m_TotalElapsedTime >= m_ChaseScatterDuration[m_CurrentCycle])
	{
		m_TotalElapsedTime -= m_ChaseScatterDuration[m_CurrentCycle];
		m_IsInChaseState = !m_IsInChaseState;
		++m_CurrentCycle;
	}
}
