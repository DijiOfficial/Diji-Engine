#include "HealthCounter.h"
#include "EventQueue.h"
//#include "Text.h"
//#include "Subject.h"

//#include <format>
diji::HealthCounter::HealthCounter(GameObject* ownerPtr, int health)
    : Component(ownerPtr)
    , m_Health{ health }
{
    
};


void diji::HealthCounter::DecreaseHealth()
{
	m_Health--;

    Notify(MessageTypes::HEALTH_CHANGE);

    if (m_Health == 0)
        SoundEventQueue::GetInstance().AddSoundRequest(SoundId::PacmanDie, 100);
}

void diji::HealthCounter::Update()
{
   
}