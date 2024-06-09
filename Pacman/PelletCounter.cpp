#include "PelletCounter.h"
#include "Observers.h"
#include "IObserver.h"
#include "GameObject.h"

void pacman::PelletCounter::Init()
{
	m_PelletObserverPtr = GetOwner()->GetComponent<PelletObserver>();
}

void pacman::PelletCounter::Update()
{
	if (not m_IsActive and m_PelletObserverPtr->GetPelletCount() > 0)
		m_IsActive = true;
	else if (not m_IsActive)
			return;

	if (not m_CanNotify and m_PelletCountAtNotification != m_PelletObserverPtr->GetPelletCount())
		m_CanNotify = true;
	else if (not m_CanNotify)
		return;

	if (m_PelletObserverPtr->GetPelletCount() % 240 == 0)
	{
		Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::LEVEL_END));
		m_CanNotify = false;
		m_PelletCountAtNotification = m_PelletObserverPtr->GetPelletCount();
	}
}

void pacman::PelletCounter::SkipLevel()
{
	Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::LEVEL_END));
}
