#include "TimerComp.h"
#include "TimeSingleton.h"

void pacman::Timer::Update()
{
	if (m_IsActive)
		m_Timer += diji::TimeSingleton::GetInstance().GetDeltaTime();
}
