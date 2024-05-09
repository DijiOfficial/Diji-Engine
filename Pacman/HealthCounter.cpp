#include "HealthCounter.h"
#include "ISoundSystem.h"
#include "Observers.h"

diji::HealthCounter::HealthCounter(GameObject* ownerPtr, int health)
    : Component(ownerPtr)
    , m_Health{ health }
{
    
};


void diji::HealthCounter::DecreaseHealth()
{
	m_Health--;

    Notify(static_cast<MessageTypes>(MessageTypesDerived::HEALTH_CHANGE));

    if (m_Health == 0)
        ServiceLocator::GetSoundSystem().AddSoundRequest(SoundId::PacmanDie, 100);
}

void diji::HealthCounter::Update()
{
   
}