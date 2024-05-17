#include "PickUp.h"
#include "Collider.h"
#include "GameObject.h"
#include "ISoundSystem.h"

#include "Observers.h"
#include "Render.h"

diji::PickUp::PickUp(GameObject* ownerPtr, const GameObject* player, const GameObject* pelletCounter, const int value)
	: Component(ownerPtr)
	, m_PelletCounter{ pelletCounter->GetComponent<PelletObserver>() }
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

	GetOwner()->GetComponent<Render>()->SetRect(GetOwner()->GetComponent<Collider>()->GetCollisionBox());
}

void diji::PickUp::HandleCollision()
{
	const auto& owner = GetOwner();
	owner->GetComponent<Render>()->DisableRender();
	Collision::GetInstance().RemoveCollider(owner->GetComponent<Collider>());

	if ((m_PelletCounter->GetPelletCount() & 1) == 0)
		ServiceLocator::GetSoundSystem().AddSoundRequest(SoundId::PelletPickUp, -1);
	else
		ServiceLocator::GetSoundSystem().AddSoundRequest(SoundId::PelletPickUp2, -1);
	
	Notify(static_cast<MessageTypes>(MessageTypesDerived::PICKUP_COLLISION));
}
