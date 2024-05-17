#include "RedAI.h"
#include "Command.h"

#include "Transform.h"
#include "Collision.h"
#include "Collider.h"
#include "TimeSingleton.h"

diji::RedAI::RedAI(GameObject* ownerPtr, GameObject* player)
	: GhostAI(ownerPtr, player)
{
	m_CurrentStateUPtr = std::make_unique<RedChase>();
	m_CurrentStateUPtr->OnEnter(this);
}

void diji::RedChase::OnEnter(const GhostAI* ghost)
{
	ghost->GetTransform()->SetMovement(diji::Movement::Left);
}

std::unique_ptr<diji::GhostState> diji::RedChase::Execute(Transform* transform, Collider* collider, [[maybe_unused]] Collider* player)
{
	auto movement = transform->GetMovement();
	auto position = transform->GetPosition();

	switch (movement)
	{
	case diji::Movement::Left:
		position.x -= 1;
		break;
	case diji::Movement::Right:
		position.x += 1;
		break;
	default:
		break;
	}

	auto shape = collider->GetCollisionBox();
	shape.left = position.x;
	shape.bottom = position.y;

	if (not Collision::GetInstance().IsCollidingWithWorld(shape))
		transform->SetPosition(position);
	else
	{
		switch (movement)
		{
			break;
		case diji::Movement::Left:
			transform->SetMovement(diji::Movement::Right);
			break;
		case diji::Movement::Right:
			transform->SetMovement(diji::Movement::Left);
			break;
		default:
			break;
		}
	}

	//const auto& test = Collision::GetInstance().IsColliding(collider);

	//for (const auto& colliders : test)
	//{
	//	if (colliders == player)
	//	{
	//		return std::make_unique<ReturnToSpawn>();
	//	}
	//}

	return nullptr;
}


//std::unique_ptr<diji::GhostState> diji::EnterMaze::Execute(Transform* transform, [[maybe_unused]] Collider* collider, [[maybe_unused]] Collider* player)
//{
//	//temp code
//	auto pos = transform->GetPosition();
//
//	if (pos.y > 300 && not tempLock)
//	{
//		pos.y -= 1;
//		transform->SetPosition(pos);
//	}
//	else if (pos.y < 300 && not tempLock)
//	{
//		pos.y += 1;
//		transform->SetPosition(pos);
//	}
//	else
//	{
//		if (pos.x < 214)
//		{
//			pos.x += 1;
//			transform->SetPosition(pos);
//		}
//		else if (pos.x > 214)
//		{
//			pos.x -= 1;
//			transform->SetPosition(pos);
//		}
//		else
//		{
//			tempLock = true;
//			pos.y -= 1;
//			transform->SetPosition(pos);
//
//			if (pos.y == 247) // When it has moved 50 pixels
//			{
//				transform->SetMovement(diji::Movement::Right);
//				return std::make_unique<RedChase>();
//			}
//		}
//	}
//
//	return nullptr;
//}