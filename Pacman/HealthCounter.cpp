#include "HealthCounter.h"
#include "ISoundSystem.h"
#include "Observers.h"

pacman::HealthCounter::HealthCounter(diji::GameObject* ownerPtr, int health)
    : Component(ownerPtr)
    , m_Health{ health }
{
    
};


void pacman::HealthCounter::DecreaseHealth()
{
	m_Health--;

    Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::HEALTH_CHANGE));

    if (m_Health == 0)
        diji::ServiceLocator::GetSoundSystem().AddSoundRequest(diji::SoundId::PacmanDie, 100);
}