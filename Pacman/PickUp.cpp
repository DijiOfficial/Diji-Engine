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

#include <iostream>
void diji::PickUp::HandleCollision()
{
	const auto& owner = GetOwner();
	owner->GetComponent<Render>()->DisableRender();
	Collision::GetInstance().RemoveCollider(owner->GetComponent<Collider>());
	ServiceLocator::GetSoundSystem().AddSoundRequest(SoundId::PelletPickUp, -1);

	//if (m_PickUpLoaderPtr->IsFirstPickUp())
	//{
	//	ServiceLocator::GetSoundSystem().AddSoundRequest(SoundId::PelletPickUp, -1);
	//	std::cout << "PelletPickUp\n";
	//}
	//else
	//{
	//	ServiceLocator::GetSoundSystem().AddSoundRequest(SoundId::PelletPickUp2, -1);
	//	std::cout << "PelletPickUp2\n";
	//}
	
	//m_PickUpLoaderPtr->SwitchPickUpState();

	Notify(static_cast<MessageTypes>(MessageTypesDerived::PICKUP_COLLISION));
}
