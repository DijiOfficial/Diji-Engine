#include "HealthCounter.h"
//#include "Text.h"
//#include "Subject.h"

#include <format>
#include <iostream>
diji::HealthCounter::HealthCounter(GameObject* ownerPtr, int health)
    : Component(ownerPtr)
    , m_Health{ health }
{
    
};


void diji::HealthCounter::DecreaseHealth()
{
	m_Health--;

    Notify(MessageTypes::HEALTH_CHANGE);
}

void diji::HealthCounter::Update()
{
   
}