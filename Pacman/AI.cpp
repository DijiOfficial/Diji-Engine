#include "AI.h"
#include "Texture.h"
#include "Transform.h"
#include "Collider.h"
#include "PickUp.h"
#include "ScoreCounter.h"
#include "GameObject.h"
#include "Observers.h"
#include "TimeSingleton.h"
#include "Render.h"
#include "ISoundSystem.h"
#include "GhostAI.h"

pacman::AI::AI(diji::GameObject* ownerPtr)
	: Component(ownerPtr)
{
	m_TextureCompPtr = nullptr;
	m_TransformCompPtr = nullptr;
	m_ColliderCompPtr = nullptr;
}

void pacman::AI::Init()
{
	const auto& ownerPtr = GetOwner();
	m_TextureCompPtr = ownerPtr->GetComponent<diji::Texture>();
	m_TransformCompPtr = ownerPtr->GetComponent<diji::Transform>();
	m_ColliderCompPtr = ownerPtr->GetComponent<diji::Collider>();

	m_TransformCompPtr->SetMovement(m_PreviousMovement);
}

void pacman::AI::Update()
{
	if (m_PauseAI)
	{
		m_PauseTime += diji::TimeSingleton::GetInstance().GetDeltaTime();
		if (m_PauseTime >= 2.f)
		{
			m_PauseAI = false;
			m_PauseTime = 0.f;
			GetOwner()->GetComponent<diji::Render>()->EnableRender();
		}
		else
			return;
	}

	//if (m_IsPoweredUp)
	//{
	//	m_PowerUpTimer += diji::TimeSingleton::GetInstance().GetDeltaTime();
	//	if (m_PowerUpTimer >= 10.f)
	//	{
	//		m_IsPoweredUp = false;
	//		m_PowerUpTimer = 0.f;
	//		diji::ServiceLocator::GetSoundSystem().AddSoundRequest(diji::SoundId::Music, -1);
	//	}
	//}

	
}
void pacman::AI::FixedUpdate()
{
	//todo: fix collision not always working
	//todo: make state machine for AI
	if (m_PauseAI)
		return;

	const auto& currentMovement = m_TransformCompPtr->GetMovement();

	const auto& shape = CalculateNewPosition(currentMovement);
	if (not diji::Collision::GetInstance().IsCollidingWithWorld(shape))
	{
		m_TransformCompPtr->SetPosition(shape.left, shape.bottom);
		if (m_PreviousMovement != currentMovement)
		{
			if (m_PreviousMovement == diji::Movement::Idle)
				m_TextureCompPtr->ResumeAnimation();
			if (currentMovement == diji::Movement::Idle)
				m_TextureCompPtr->PauseAnimation();
			else
				m_TextureCompPtr->SetRotationAngle(static_cast<int>(currentMovement) * 90.f);

			m_PreviousMovement = currentMovement;
		}

		if (currentMovement != diji::Movement::Idle)
			m_TransformCompPtr->SetLookingDirection(currentMovement);
	}
	else
	{
		const auto& oldShape = CalculateNewPosition(m_PreviousMovement);

		if (not diji::Collision::GetInstance().IsCollidingWithWorld(oldShape))
			m_TransformCompPtr->SetPosition(oldShape.left, oldShape.bottom);
		else
			m_TransformCompPtr->SetMovement(diji::Movement::Idle);

		if (m_PreviousMovement != diji::Movement::Idle)
			m_TransformCompPtr->SetLookingDirection(m_PreviousMovement);
	}
	// late update stuff
	if (currentMovement == diji::Movement::Left)
		if (shape.left < 0 - shape.width)
			m_TransformCompPtr->SetPosition(TOTAL_WIDTH, shape.bottom);
	if (currentMovement == diji::Movement::Right)
		if (shape.left > TOTAL_WIDTH)
			m_TransformCompPtr->SetPosition(0 - shape.width, shape.bottom);

	//std::cout << "AI: " << shape.left << " " << shape.bottom << std::endl;

	//if (Collision::GetInstance().IsCollidingWithIntersection(shape))
	//{
	//	std::cout << "AI: Intersection Collision" << std::endl;
	//}

}

#include <iostream>
void pacman::AI::OnNotify(diji::MessageTypes message, [[maybe_unused]] diji::Subject* subject)
{
	auto msg = static_cast<MessageTypesDerived>(message);
	switch (msg)
	{
	case MessageTypesDerived::POWERUP_COLLISION:
	{
		m_GhostsEaten = 0;
		[[fallthrough]];
	}
	case MessageTypesDerived::PICKUP_COLLISION:
	{
		PickUp* pickUp = dynamic_cast<PickUp*>(subject);

		const int value = pickUp->GetValue();
		GetOwner()->GetComponent<ScoreCounter>()->IncreaseScore(value);
		break;
	}
	case MessageTypesDerived::ENEMY_COLLISION:
	{
		constexpr int EATEN_GHOST_POINTS = 200;

		if (IsGhostFrightened())
		{
			m_PauseAI = true;
			GetOwner()->GetComponent<diji::Render>()->DisableRender();
			const int value = EATEN_GHOST_POINTS * static_cast<int>(std::pow(2, m_GhostsEaten));
			GetOwner()->GetComponent<ScoreCounter>()->IncreaseScore(value);
			++m_GhostsEaten;
		}
		else
			std::cout << "Game Over" << std::endl;
		break;
	}
	default:
		break;
	}
}

//todo: clean up
const diji::Rectf pacman::AI::CalculateNewPosition(diji::Movement movement)
{
	auto shape = m_ColliderCompPtr->GetCollisionBox();
	//const auto& deltaTime = TimeSingleton::GetInstance().GetDeltaTime();
	switch (movement)
	{
	case diji::Movement::Up:
		//shape.bottom -= m_Speed.y * deltaTime;
		//--shape.bottom;
		shape.bottom -= 2;
		break;
	case diji::Movement::Down:
		//shape.bottom += m_Speed.y * deltaTime;
		//++shape.bottom;
		shape.bottom += 2;
		break;
	case diji::Movement::Left:
		//shape.left -= m_Speed.x * deltaTime;
		//--shape.left;
		shape.left -= 2;
		break;
	case diji::Movement::Right:
		//shape.left += m_Speed.x * deltaTime;
		//++shape.left;
		shape.left += 2;
		break;
	}

	if (diji::Collision::GetInstance().IsCollidingWithWorld(shape))
	{
		shape.left = std::round(shape.left);
		shape.bottom = std::round(shape.bottom);
		switch (movement)
		{
		case diji::Movement::Up:
			++shape.bottom;
			break;
		case diji::Movement::Down:
			--shape.bottom;
			break;
		case diji::Movement::Left:
			++shape.left;
			break;
		case diji::Movement::Right:
			--shape.left;
			break;
		}
	}

	return shape;
}

bool pacman::AI::IsGhostFrightened() const //super convoluted way of checking if the ghost is frightened, If ghost is subject I could dynamic cast it in the Notify
{
	auto colliders = diji::Collision::GetInstance().IsColliding(m_ColliderCompPtr);
	colliders.erase(std::remove_if(colliders.begin(), colliders.end(),
		[](const auto& collider)
		{
			return !collider->GetParent()->HasComponent<GhostAI>();
		}), colliders.end());

	if (colliders.empty())
		return false;

	if (colliders.size() == 1)
	{
		auto ghostAI = colliders.front()->GetParent()->GetComponent<GhostAI>();
		return ghostAI->IsFrightened();
	}

	const auto& playerCollider = m_ColliderCompPtr->GetCollisionBox();
	glm::vec2 playerCenter(playerCollider.left + playerCollider.width * 0.5f, playerCollider.bottom + playerCollider.height * 0.5f);

	auto closestCollider = colliders.front();
	float minDistance = std::numeric_limits<float>::max();

	for (const auto& collider : colliders)
	{
		const auto& ghostCollider = collider->GetCollisionBox();
		glm::vec2 ghostCenter(ghostCollider.left + ghostCollider.width * 0.5f, ghostCollider.bottom + ghostCollider.height * 0.5f);

		const float distance = glm::distance(playerCenter, ghostCenter);

		if (distance < minDistance)
		{
			minDistance = distance;
			closestCollider = collider;
		}
	}

	auto ghostAI = closestCollider->GetParent()->GetComponent<GhostAI>();
	return ghostAI->IsFrightened();
}

//const diji::Rectf pacman::AI::CalculateNewPosition(diji::Movement movement)
//{
//	//auto shape = m_ColliderCompPtr->GetCollisionBox() + m_TransformCompPtr->GetMovementVector(2.f);
//
//	const auto position = m_TransformCompPtr->GetPosition() + m_TransformCompPtr->GetMovementVector(2.f);
//	auto shape = m_ColliderCompPtr->GetCollisionBox();
//	shape.left = position.x;
//	shape.bottom = position.y;
//
//	if (diji::Collision::GetInstance().IsCollidingWithWorld(shape))
//	{
//		shape.left = std::round(shape.left);
//		shape.bottom = std::round(shape.bottom);
//		switch (movement)
//		{
//		case diji::Movement::Up:
//			++shape.bottom;
//			break;
//		case diji::Movement::Down:
//			--shape.bottom;
//			break;
//		case diji::Movement::Left:
//			++shape.left;
//			break;
//		case diji::Movement::Right:
//			--shape.left;
//			break;
//		}
//	}
//
//	return shape;
//}