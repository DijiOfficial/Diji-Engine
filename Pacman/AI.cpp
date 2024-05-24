#include "AI.h"
#include "Texture.h"
#include "Transform.h"
#include "Collider.h"
#include "PickUp.h"
#include "ScoreCounter.h"
#include "GameObject.h"
#include "Observers.h"
#include "TimeSingleton.h"

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

void pacman::AI::FixedUpdate()
{
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
	}
	else
	{
		const auto& oldShape = CalculateNewPosition(m_PreviousMovement);

		if (not diji::Collision::GetInstance().IsCollidingWithWorld(oldShape))
			m_TransformCompPtr->SetPosition(oldShape.left, oldShape.bottom);
		else
		{
			m_TransformCompPtr->SetMovement(diji::Movement::Idle);
			// Smooth out collision (testing)
			//SmoothOutCollision(shape, m_PreviousMovement);
		}
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

void pacman::AI::OnNotify(diji::MessageTypes message, [[maybe_unused]] diji::Subject* subject)
{
	auto msg = static_cast<MessageTypesDerived>(message);
	switch (msg)
	{
	case MessageTypesDerived::LEVEL_COLLISION:
	{
		//std::cout << "AI: Level Collision" << std::endl;
		break;
	}
	case MessageTypesDerived::PICKUP_COLLISION:
	case MessageTypesDerived::POWERUP_COLLISION:
	{
		PickUp* pickUp = dynamic_cast<PickUp*>(subject);

		const int value = pickUp->GetValue();
		GetOwner()->GetComponent<ScoreCounter>()->IncreaseScore(value);
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