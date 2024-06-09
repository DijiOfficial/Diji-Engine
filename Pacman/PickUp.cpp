#include "PickUp.h"
#include "Collider.h"
#include "GameObject.h"
#include "ISoundSystem.h"
#include "Observers.h"
#include "Render.h"

pacman::PickUp::PickUp(diji::GameObject* ownerPtr, const diji::GameObject* player, const diji::GameObject* player2, const diji::GameObject* pelletCounter, const int value)
	: Component(ownerPtr)
	, m_Value{ value }
{
	if (value == 10)
		m_SoundId = SoundId::PelletPickUp;
	else if (value == 50)
		m_SoundId = SoundId::PowerPellet;
	else
		m_SoundId = SoundId::InvalidSoundId;
	
	m_PelletCounter = pelletCounter ? pelletCounter->GetComponent<PelletObserver>() : nullptr;
	m_PlayerColliderPtr = player->GetComponent<diji::Collider>();
	m_Player2ColliderPtr = player2 ? player2->GetComponent<diji::Collider>() : nullptr;
	m_OwnerColliderPtr = nullptr;
	m_RenderCompPtr = nullptr;
}

pacman::PickUp::PickUp(diji::GameObject* ownerPtr, const diji::GameObject* player, const diji::GameObject* player2, const int value)
	: PickUp(ownerPtr, player, player2, nullptr, value)
{
	m_IsPowerUp = true;
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
	//todo: check collisions on late update
	const auto& colliders = diji::Collision::GetInstance().IsColliding(m_OwnerColliderPtr);
	for (const auto& collider : colliders)
	{
		if (collider == m_PlayerColliderPtr or collider == m_Player2ColliderPtr)
		{
			const auto& playerBox = collider == m_PlayerColliderPtr ? m_PlayerColliderPtr->GetCollisionBox() : m_Player2ColliderPtr->GetCollisionBox();
			const glm::vec2 playerCenter = { playerBox.left + playerBox.width * 0.5f, playerBox.bottom + playerBox.height * 0.5f };

			const auto& pelletBox = m_OwnerColliderPtr->GetCollisionBox();
			const glm::vec2 pelletCenter = { pelletBox.left + pelletBox.width * 0.5f, pelletBox.bottom + pelletBox.height * 0.5f };

			const float deltaX = playerCenter.x - pelletCenter.x;
			const float deltaY = playerCenter.y - pelletCenter.y;
			const float distance = deltaX * deltaX + deltaY * deltaY;

			if (distance <= 64.f)
				HandleCollision();

			break;
		}
	}
}

void pacman::PickUp::OnNotify(diji::MessageTypes message, diji::Subject*)
{
	auto msg = static_cast<MessageTypesDerived>(message);
	if (msg == MessageTypesDerived::LEVEL_END)
	{
		m_PowerUpInvisibleFrames = 0;
		m_IsDisabled = false;
		m_RenderCompPtr->EnableRender();
		const auto& collider = GetOwner()->GetComponent<diji::Collider>();
		diji::Collision::GetInstance().AddCollider(collider, collider->GetCollisionBox());
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
	case SoundId::PelletPickUp:
		if ((m_PelletCounter->GetPelletCount() & 1) == 0)
			diji::ServiceLocator::GetSoundSystem().AddSoundRequest("munch_1.wav", false, -1);
		else
			diji::ServiceLocator::GetSoundSystem().AddSoundRequest("munch_2.wav", false, -1);
		
		Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::PICKUP_COLLISION));
		break;

	case SoundId::PowerPellet:
		diji::ServiceLocator::GetSoundSystem().AddSoundRequest("power_pellet.wav", true, -1);
		
		Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::POWERUP_COLLISION));
		break;
	}
	
}
