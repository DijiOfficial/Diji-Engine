#include "GhostsAlgorithm.h"
#include "TimeSingleton.h"
#include "Observers.h"

void pacman::GhostsTimers::Update()
{
	if (m_IsInIntro)
		return;

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

	if (m_CurrentCycle > static_cast<int>(m_ChaseScatterDuration.size()) - 1)
		return;

	m_TotalElapsedTime += deltaTime;
	if (m_TotalElapsedTime >= m_ChaseScatterDuration[m_CurrentCycle])
	{
		m_TotalElapsedTime -= m_ChaseScatterDuration[m_CurrentCycle];
		m_IsInChaseState = !m_IsInChaseState;
		++m_CurrentCycle;
	}
}

void pacman::GhostsTimers::OnNotify(diji::MessageTypes message, diji::Subject*)
{
	auto msg = static_cast<MessageTypesDerived>(message);
	switch (msg)
	{
	case MessageTypesDerived::LEVEL_START:
	{
		m_IsInIntro = false;
		break;
	}
	default:
		break;
	}
}
