#include "GhostCollision.h"
#include "Collision.h"
#include "Collider.h"
#include "GameObject.h"
#include "Observers.h"
#include "GhostAI.h"
#include "GhostStates.h"
#include "AI.h"

pacman::GhostCollision::GhostCollision(diji::GameObject* ownerPtr, const diji::GameObject* player)
	: Component(ownerPtr)
	, m_PlayerColliderPtr{ player->GetComponent<diji::Collider>() }
	, m_OwnerColliderPtr{ nullptr }
	, m_PlayerAIPtr{ player->GetComponent<AI>() }
{
};

void pacman::GhostCollision::Init()
{
	m_OwnerColliderPtr = GetOwner()->GetComponent<diji::Collider>();
}

void pacman::GhostCollision::Update()
{
	//todo: late update
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

			if (distance <= 250.f)
			{
				const auto& currentState = GetOwner()->GetComponent<GhostAI>()->GetCurrentState();
				if (dynamic_cast<const pacman::Respawn*>(currentState) == nullptr &&
					dynamic_cast<const pacman::Eaten*>(currentState) == nullptr &&
					dynamic_cast<const pacman::Dying*>(currentState) == nullptr)
				{
					Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::ENEMY_COLLISION));
				}
			}

			break;
		}
	}
}

pacman::GhostState* pacman::GhostCollision::GetCurrentState()
{
	return GetOwner()->GetComponent<GhostAI>()->GetCurrentState();
}

int pacman::GhostCollision::GetGhostsEaten() const
{
	return m_PlayerAIPtr->GetGhostsEaten();
}
