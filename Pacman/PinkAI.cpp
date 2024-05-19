#include "PinkAI.h"

#include "Collision.h"
#include "Collider.h"
#include "TimeSingleton.h"
#include "Command.h"
#include "Transform.h"

diji::PinkAI::PinkAI(GameObject* ownerPtr, GameObject* player)
	: GhostAI(ownerPtr, player)
{
	m_CurrentStateUPtr = std::make_unique<Waiting>();
	m_CurrentStateUPtr->OnEnter(this);
}

void diji::PinkAI::FixedUpdate()
{
	auto state = m_CurrentStateUPtr->Execute(this);

	if (state)
	{
		m_CurrentStateUPtr->OnExit(this);
		m_CurrentStateUPtr = std::move(state);
		m_CurrentStateUPtr->OnEnter(this);
	}
}

std::unique_ptr<diji::GhostState> diji::Waiting::Execute(const GhostAI* ghost)
{
	//temp code
	const auto& transform = ghost->GetTransform();
	const auto& collider = ghost->GetCollider();

	tempTimer += TimeSingleton::GetInstance().GetDeltaTime();
	if (tempTimer > 5)
	{
		//return std::make_unique<EnterMaze>(m_TransformCompPtr, m_ColliderCompPtr, m_PlayerColliderPtr);
		return std::make_unique<EnterMaze>();
	}
	auto movement = transform->GetMovement();
	auto position = transform->GetPosition();
	
	switch (movement)
	{
	case diji::Movement::Up:
		position.y -= 1;
		break;
	case diji::Movement::Down:
		position.y += 1;
		break;
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
		case diji::Movement::Up:
			transform->SetMovement(diji::Movement::Right);
			break;
		case diji::Movement::Down:
			transform->SetMovement(diji::Movement::Left);
			break;
		case diji::Movement::Left:
			transform->SetMovement(diji::Movement::Up);
			break;
		case diji::Movement::Right:
			transform->SetMovement(diji::Movement::Down);
			break;
		default:
			break;
		}
	}

	return nullptr;
}

std::unique_ptr<diji::GhostState> diji::ChasePac::Execute(const GhostAI* ghost)
{
	const auto& transform = ghost->GetTransform();
	const auto& collider = ghost->GetCollider();
	const auto& player = ghost->GetPlayerCollider();

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

	const auto& test = Collision::GetInstance().IsColliding(collider);

	for (const auto& colliders : test)
	{
		if (colliders == player)
		{
			return std::make_unique<ReturnToSpawn>();
		}
	}

	return nullptr;
}

std::unique_ptr<diji::GhostState> diji::ReturnToSpawn::Execute(const GhostAI* ghost)
{
	//temp code
	const auto& transform = ghost->GetTransform();

	auto pos = transform->GetPosition();

	if (pos.y > 300)
	{
		pos.y -= 1;
		transform->SetPosition(pos);
	}
	else if (pos.y < 300)
	{
		pos.y += 1;
		transform->SetPosition(pos);
	}
	else
	{
		if (pos.x < 214)
		{
			pos.x += 1;
			transform->SetPosition(pos);
		}
		else if (pos.x > 214)
		{
			pos.x -= 1;
			transform->SetPosition(pos);
		}
		else
		{
			return std::make_unique<EnterMaze>();
		}
	}
	return nullptr;
}

std::unique_ptr<diji::GhostState> diji::EnterMaze::Execute(const GhostAI* ghost)
{
	//temp code
	const auto& transform = ghost->GetTransform();

	auto pos = transform->GetPosition();

	if (pos.y > 300 && not tempLock)
	{
		pos.y -= 1;
		transform->SetPosition(pos);
	}
	else if (pos.y < 300 && not tempLock)
	{
		pos.y += 1;
		transform->SetPosition(pos);
	}
	else
	{
		if (pos.x < 214)
		{
			pos.x += 1;
			transform->SetPosition(pos);
		}
		else if (pos.x > 214)
		{
			pos.x -= 1;
			transform->SetPosition(pos);
		}
		else
		{
			tempLock = true;
			pos.y -= 1;
			transform->SetPosition(pos);

			if (pos.y == 247) // When it has moved 50 pixels
			{
				transform->SetMovement(diji::Movement::Right);
				return std::make_unique<ChasePac>();
			}
		}
	}

	return nullptr;
}