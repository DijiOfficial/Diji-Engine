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
#include <iostream>
std::unique_ptr<diji::GhostState> diji::RedChase::Execute(Transform* transform, Collider* collider, [[maybe_unused]] Collider* player)
{
	auto shape = collider->GetCollisionBox();

	if (Collision::GetInstance().IsCollidingWithIntersection(shape) and not m_TempLock)
	{
		CalculateDirection(transform, collider, player);
		m_TempLock = true;
	}
	else if (m_TempLock)
	{
		m_LockedFrames++;
		if (m_LockedFrames >= 5)
		{
			m_TempLock = false;
			m_LockedFrames = 0;
		}
	}

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
	case diji::Movement::Up:
		position.y -= 1;
		break;
	case diji::Movement::Down:
		position.y += 1;
		break;
	default:
		break;
	}

	shape.left = position.x;
	shape.bottom = position.y;

	if (not Collision::GetInstance().IsCollidingWithWorld(shape))
		transform->SetPosition(position);

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

void diji::RedChase::CalculateDirection(Transform* transform, Collider* collider, Collider* player)
{
	const auto& playerPos = player->GetCollisionBox();
	const auto& shape = collider->GetCollisionBox();
	std::map<diji::Movement, bool> possibleDirections = {
		{diji::Movement::Up, true},
		{diji::Movement::Left, true},
		{diji::Movement::Down, true},
		{diji::Movement::Right, true}
	};

	//remove the opposite direction from which you came
	possibleDirections[static_cast<diji::Movement>((static_cast<int>(transform->GetMovement()) + 2) % 4)] = false;

	//remove directions that are blocked
	const glm::vec2 center(shape.left + shape.width *0.5f, shape.bottom + shape.height *0.5f);

	for (auto& [direction, canMove] : possibleDirections)
	{
		if (!canMove) continue;

		const glm::vec2 newPosition = center + GetTargetTranslation(direction);

		//const diji::Rectf newShape = { newPosition.x - shape.width * 0.5f, newPosition.y - shape.height * 0.5f, 2.f, 2.f };

		if (Collision::GetInstance().IsCollidingWithWorld(center, newPosition))
			canMove = false;
	}

	//check the remaining directions to determine the closest one
	Movement bestDirection = Movement::Up;
	float smallestDistance = std::numeric_limits<float>::max();

	const glm::vec2 playerCenter(playerPos.left + playerPos.width * 0.5f, playerPos.bottom + playerPos.height * 0.5f);
	for (const auto& [direction, canMove] : possibleDirections)
	{
		if (!canMove) continue;

		const glm::vec2 newPosition = center + GetTargetTranslation(direction);
		const float distance = glm::distance(newPosition, playerCenter);

		if (distance < smallestDistance)
		{
			smallestDistance = distance;
			bestDirection = direction;
		}
		else if (distance == smallestDistance)
		{
			// Prioritize the order Up, Left, Down, Right
			if ((bestDirection == Movement::Up && direction != Movement::Up) ||
				(bestDirection == Movement::Left && direction != Movement::Up && direction != Movement::Left) ||
				(bestDirection == Movement::Down && direction != Movement::Up && direction != Movement::Left && direction != Movement::Down))
			{
				continue;
			}
			bestDirection = direction;
		}
	}

	transform->SetMovement(bestDirection);
}

glm::vec2 diji::RedChase::GetTargetTranslation(Movement movement) const
{
	glm::vec2 translation{ 0, 0 };
	switch (movement)
	{
	case diji::Movement::Right:
		translation.x = 16;
		break;
	case diji::Movement::Down:
		translation.y = 16;
		break;
	case diji::Movement::Left:
		translation.x = -16;
		break;
	case diji::Movement::Up:
		translation.y = -16;
		break;
	}

	return translation;
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