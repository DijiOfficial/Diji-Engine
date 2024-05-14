#include "PinkAI.h"
#include "GameObject.h"

#include "Transform.h"
#include "Collision.h"
#include "Collider.h"
#include "TimeSingleton.h"

diji::PinkAI::PinkAI(GameObject* ownerPtr, GameObject* player)
	: Component(ownerPtr)
	, m_PlayerColliderPtr{ player->GetComponent<Collider>() }
{
	m_TransformCompPtr = ownerPtr->GetComponent<Transform>();
	m_ColliderCompPtr = ownerPtr->GetComponent<Collider>();

	assert(m_TransformCompPtr and "AI Component needs to be initialized aftera Transform");
	assert(m_ColliderCompPtr and "AI Component needs to be initialized aftera Collider");

	m_TransformCompPtr->SetMovement(Movement::Up);

	m_CurrentStateUPtr = std::make_unique<Waiting>();
}

void diji::PinkAI::Update()
{
	auto state = m_CurrentStateUPtr->Execute(m_TransformCompPtr, m_ColliderCompPtr, m_PlayerColliderPtr);

	if (state)
		m_CurrentStateUPtr = std::move(state);
}

std::unique_ptr<diji::GhostState> diji::Waiting::Execute(Transform* transform, Collider* collider, [[maybe_unused]] Collider* player)
{
	//temp code
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

//diji::State::State(Transform* transform, Collider* collider, Collider* player)
//	: m_TransformCompPtr{ transform }
//	, m_ColliderCompPtr{ collider }
//	, m_PlayerColliderPtr{ player }
//{
//}

std::unique_ptr<diji::GhostState> diji::EnterMaze::Execute(Transform* transform, [[maybe_unused]] Collider* collider, [[maybe_unused]] Collider* player)
{
	//temp code
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

std::unique_ptr<diji::GhostState> diji::ChasePac::Execute(Transform* transform, Collider* collider, Collider* player)
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

std::unique_ptr<diji::GhostState> diji::ReturnToSpawn::Execute(Transform* transform, [[maybe_unused]] Collider* collider, [[maybe_unused]] Collider* player)
{
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
