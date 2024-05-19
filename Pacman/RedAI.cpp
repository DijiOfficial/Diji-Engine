#include "RedAI.h"
#include "Command.h"

#include "Transform.h"
#include "Collision.h"
#include "Collider.h"
#include "TimeSingleton.h"

diji::RedAI::RedAI(GameObject* ownerPtr, GameObject* player)
	: GhostAI(ownerPtr, player)
{
	m_PersonnalSpawn = { 212, 300 };
	m_ScatterTarget = { 450, 60 };

	m_CurrentStateUPtr = std::make_unique<Scatter>();
	GetTransform()->SetMovement(Movement::Right);
	m_CurrentStateUPtr->OnEnter(this);
}

//void diji::RedChase::OnEnter(const GhostAI* ghost)
//{
//	ghost->TurnAround();
//}
//
//std::unique_ptr<diji::GhostState> diji::RedChase::Execute(const GhostAI* ghost)
//{
//	const auto& transform = ghost->GetTransform();
//	const auto& collider = ghost->GetCollider();
//	const auto& player = ghost->GetPlayerCollider();
//
//	const auto& playerPos = player->GetCollisionBox();
//	const glm::vec2 target(playerPos.left + playerPos.width * 0.5f, playerPos.bottom + playerPos.height * 0.5f);
//
//	SeekTarget(ghost, target);
//
//	const auto position = transform->GetPosition() + transform->GetMovementVector(m_Step);
//
//	auto shape = collider->GetCollisionBox();
//	shape.left = position.x;
//	shape.bottom = position.y;
//
//	if (not Collision::GetInstance().IsCollidingWithWorld(shape)) //check for collision just in case ghost goes over intersection trigger
//		transform->SetPosition(position);
//
//	//collide player test
//	//const auto& test = Collision::GetInstance().IsColliding(collider);
//	//for (const auto& colliders : test)
//	//{
//	//	if (colliders == player)
//	//	{
//	//		return std::make_unique<Eaten>();
//	//	}
//	//}
//
//	return nullptr;
//}