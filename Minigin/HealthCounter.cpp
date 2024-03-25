#include "HealthCounter.h"
#include "Text.h"

#include <format>
#include <iostream>
diji::HealthCounter::HealthCounter(GameObject* ownerPtr, int health)
    : Component(ownerPtr)
    , m_Health{ health }
{
    
};


void diji::HealthCounter::Hit()
{
	m_Health--;

    auto& subject = ISubject::GetInstance();
    subject.Notify(GetOwner(), MessageTypes::ENTITY_COLLISION);
}

void diji::HealthCounter::Update()
{
   
}