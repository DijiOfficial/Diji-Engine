#include "PickUp.h"
#include "Collider.h"
#include "GameObject.h"
#include "EventQueue.h"

diji::PickUp::PickUp(GameObject* ownerPtr, const int value)
	: Component(ownerPtr)
	, m_Value{ value }
{
	if (value == 10)
		m_SoundId = SoundId::PelletPickUp;
	else
		m_SoundId = SoundId::InvalidSoundId;
	//else if (value == 50)
	//	m_SoundId = SoundId::PowerPelletPickUp;
}

void diji::PickUp::Update()
{

}

void diji::PickUp::HandleCollision()
{
	Collision::GetInstance().RemoveCollider(GetOwner()->GetComponent<Collider>());
	
	if (SoundEventQueue::GetInstance().IsFirstPickUp())
		SoundEventQueue::GetInstance().AddSoundRequest(SoundId::PelletPickUp, -1);
	else
		SoundEventQueue::GetInstance().AddSoundRequest(SoundId::PelletPickUp2, -1);
	
	SoundEventQueue::GetInstance().SwitchPickUpState();

	Notify(MessageTypes::PICKUP_COLLISION);
}
