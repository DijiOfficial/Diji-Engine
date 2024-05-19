#include "PickUp.h"
#include "Collider.h"
#include "GameObject.h"
#include "ISoundSystem.h"

#include "Observers.h"
#include "Render.h"

diji::PickUp::PickUp(GameObject* ownerPtr, const GameObject* player, const GameObject* pelletCounter, const int value)
	: Component(ownerPtr)
	, m_Value{ value }
{
	if (value == 10)
		m_SoundId = SoundId::PelletPickUp;
	else if (value == 50)
		m_SoundId = SoundId::PowerPellet;
	else
		m_SoundId = SoundId::InvalidSoundId;

	m_PlayerColliderPtr = player->GetComponent<Collider>();
	m_OwnerColliderPtr = ownerPtr->GetComponent<Collider>();
	m_RenderCompPtr = ownerPtr->GetComponent<Render>();
	
	m_PelletCounter = pelletCounter ? pelletCounter->GetComponent<PelletObserver>() : nullptr;
}

diji::PickUp::PickUp(GameObject* ownerPtr, const GameObject* player, const int value) 
	: PickUp(ownerPtr, player, nullptr, value)
{
	m_IsPowerUp = { true };
}

void diji::PickUp::Update()
{
	if (m_IsDisabled)
		return;

	if (m_IsPowerUp)
	{
		++m_PowerUpInvisibleFrames;
		if (m_PowerUpInvisibleFrames == 12)
		{
			m_RenderCompPtr->DisableRender();
		}
		else if (m_PowerUpInvisibleFrames > 24)
		{
			m_RenderCompPtr->EnableRender();
			m_PowerUpInvisibleFrames = 0;
		}
	}

	//this is rather slow, possible optimisation?
	const auto& colliders = Collision::GetInstance().IsColliding(m_OwnerColliderPtr);
	for (const auto& collider : colliders)
	{
		if (collider == m_PlayerColliderPtr)
		{
			//todo: check if the player is within 16px of the pellet
			HandleCollision();
		}
	}
}

void diji::PickUp::HandleCollision()
{
	m_IsDisabled = true;
	const auto& owner = GetOwner();
	m_RenderCompPtr->DisableRender();
	Collision::GetInstance().RemoveCollider(owner->GetComponent<Collider>());

	switch (m_SoundId)
	{
	case diji::SoundId::PelletPickUp:
		if ((m_PelletCounter->GetPelletCount() & 1) == 0)
			ServiceLocator::GetSoundSystem().AddSoundRequest(SoundId::PelletPickUp, -1);
		else
			ServiceLocator::GetSoundSystem().AddSoundRequest(SoundId::PelletPickUp2, -1);
		
		Notify(static_cast<MessageTypes>(MessageTypesDerived::PICKUP_COLLISION));
		break;

	case diji::SoundId::PowerPellet:
		ServiceLocator::GetSoundSystem().AddSoundRequest(m_SoundId, -1); 
		
		Notify(static_cast<MessageTypes>(MessageTypesDerived::POWERUP_COLLISION));
		break;
	}
	
}
