#include "PickUp.h"
#include "Collider.h"
#include "GameObject.h"
#include "ISoundSystem.h"

#include "Observers.h"
#include "Render.h"

pacman::PickUp::PickUp(diji::GameObject* ownerPtr, const diji::GameObject* player, const diji::GameObject* pelletCounter, const int value)
	: Component(ownerPtr)
	, m_Value{ value }
{
	if (value == 10)
		m_SoundId = diji::SoundId::PelletPickUp;
	else if (value == 50)
		m_SoundId = diji::SoundId::PowerPellet;
	else
		m_SoundId = diji::SoundId::InvalidSoundId;
	
	m_OwnerColliderPtr = nullptr;
	m_RenderCompPtr = nullptr;
	
	m_PlayerColliderPtr = player->GetComponent<diji::Collider>();
	m_PelletCounter = pelletCounter ? pelletCounter->GetComponent<PelletObserver>() : nullptr;
}

pacman::PickUp::PickUp(diji::GameObject* ownerPtr, const diji::GameObject* player, const int value)
	: PickUp(ownerPtr, player, nullptr, value)
{
	m_IsPowerUp = { true };
}

void pacman::PickUp::Init()
{
	const auto& ownerPtr = GetOwner();
	m_OwnerColliderPtr = ownerPtr->GetComponent<diji::Collider>();
	m_RenderCompPtr = ownerPtr->GetComponent<diji::Render>();
}

void pacman::PickUp::Update()
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
	const auto& colliders = diji::Collision::GetInstance().IsColliding(m_OwnerColliderPtr);
	for (const auto& collider : colliders)
	{
		if (collider == m_PlayerColliderPtr)
		{
			//todo: check if the player is within 16px of the pellet
			HandleCollision();
		}
	}
}

void pacman::PickUp::HandleCollision()
{
	m_IsDisabled = true;
	const auto& owner = GetOwner();
	m_RenderCompPtr->DisableRender();
	diji::Collision::GetInstance().RemoveCollider(owner->GetComponent<diji::Collider>());

	switch (m_SoundId)
	{
	case diji::SoundId::PelletPickUp:
		if ((m_PelletCounter->GetPelletCount() & 1) == 0)
			diji::ServiceLocator::GetSoundSystem().AddSoundRequest(diji::SoundId::PelletPickUp, -1);
		else
			diji::ServiceLocator::GetSoundSystem().AddSoundRequest(diji::SoundId::PelletPickUp2, -1);
		
		Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::PICKUP_COLLISION));
		break;

	case diji::SoundId::PowerPellet:
		diji::ServiceLocator::GetSoundSystem().AddSoundRequest(m_SoundId, -1);
		
		Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::POWERUP_COLLISION));
		break;
	}
	
}
