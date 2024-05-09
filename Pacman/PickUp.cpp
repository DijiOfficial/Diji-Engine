#include "PickUp.h"
#include "Collider.h"
#include "GameObject.h"
#include "ISoundSystem.h"

#include "Observers.h"
#include "Render.h"
diji::PickUp::PickUp(GameObject* ownerPtr, GameObject* player, const int value)
	: Component(ownerPtr)
	, m_Value{ value }
{
	m_PlayerColliderPtr = player->GetComponent<Collider>();
	m_OwnerColliderPtr = ownerPtr->GetComponent<Collider>();
	if (value == 10)
		m_SoundId = SoundId::PelletPickUp;
	else
		m_SoundId = SoundId::InvalidSoundId;
	//else if (value == 50)
	//	m_SoundId = SoundId::PowerPelletPickUp;
}

void diji::PickUp::Update()
{
	const auto& colliders = Collision::GetInstance().IsColliding(m_OwnerColliderPtr);
	for (const auto& collider : colliders)
	{
		if (collider == m_PlayerColliderPtr)
		{
			HandleCollision();
		}
	}
}

void diji::PickUp::HandleCollision()
{
	const auto& owner = GetOwner();
	owner->GetComponent<Render>()->DisableRender();
	Collision::GetInstance().RemoveCollider(owner->GetComponent<Collider>());
	
	//if (ServiceLocator::GetSoundSystem().IsFirstPickUp())
	//	SoundEventQueue::GetInstance().AddSoundRequest(SoundId::PelletPickUp, -1);
	//else
		ServiceLocator::GetSoundSystem().AddSoundRequest(SoundId::PelletPickUp2, -1);
	
	//SoundEventQueue::GetInstance().SwitchPickUpState();

	Notify(static_cast<MessageTypes>(MessageTypesDerived::PICKUP_COLLISION));
}
