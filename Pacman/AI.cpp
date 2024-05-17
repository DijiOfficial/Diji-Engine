#include "AI.h"
#include "Texture.h"
#include "Transform.h"
#include "Collider.h"
#include "PickUp.h"
#include "ScoreCounter.h"

#include "Observers.h"
#include "TimeSingleton.h"
#include <iostream>
#include "Render.h"
diji::AI::AI(GameObject* ownerPtr)
	: Component(ownerPtr)
{
	m_TextureCompPtr = ownerPtr->GetComponent<Texture>();
	m_TransformCompPtr = ownerPtr->GetComponent<Transform>();
	m_ColliderCompPtr = ownerPtr->GetComponent<Collider>();
	assert(m_TextureCompPtr and "AI Component needs to be initialized aftera Texture");
	assert(m_TransformCompPtr and "AI Component needs to be initialized aftera Transform");
	assert(m_ColliderCompPtr and "AI Component needs to be initialized aftera Collider");

	m_TransformCompPtr->SetMovement(m_PreviousMovement);
	//m_CurrentStateUPtr = std::make_unique<Idle>();
	//m_CurrentStateUPtr->OnEnter();
}

void diji::AI::Update()
{
	//auto state = m_CurrentStateUPtr->Execute(m_TransformCompPtr, m_ColliderCompPtr);
	//if (state)
	//{
	//	m_CurrentStateUPtr->OnExit();
	//	m_CurrentStateUPtr = std::move(state);
	//	m_CurrentStateUPtr->OnEnter();
	//}
	//const auto& currentMovement = m_TransformCompPtr->GetMovement();
	//const auto& shape = CalculateNewPosition(currentMovement);
}

void diji::AI::FixedUpdate()
{
	const auto& currentMovement = m_TransformCompPtr->GetMovement();

	const auto& shape = CalculateNewPosition(currentMovement);
	if (not Collision::GetInstance().IsCollidingWithWorld(shape))
	{
		m_TransformCompPtr->SetPosition(shape.left, shape.bottom);
		if (m_PreviousMovement != currentMovement)
		{
			if (m_PreviousMovement == Movement::Idle)
				m_TextureCompPtr->ResumeAnimation();
			if (currentMovement == Movement::Idle)
				m_TextureCompPtr->PauseAnimation();
			else
				m_TextureCompPtr->SetRotationAngle(static_cast<int>(currentMovement) * 90.f);

			m_PreviousMovement = currentMovement;
		}
	}
	else
	{
		const auto& oldShape = CalculateNewPosition(m_PreviousMovement);

		if (not Collision::GetInstance().IsCollidingWithWorld(oldShape))
			m_TransformCompPtr->SetPosition(oldShape.left, oldShape.bottom);
		else
		{
			m_TransformCompPtr->SetMovement(Movement::Idle);
			// Smooth out collision (testing)
			//SmoothOutCollision(shape, m_PreviousMovement);
		}
	}

	// late update stuff
	if (currentMovement == Movement::Left)
		if (shape.left < 0 - shape.width)
			m_TransformCompPtr->SetPosition(TOTAL_WIDTH, shape.bottom);
	if (currentMovement == Movement::Right)
		if (shape.left > TOTAL_WIDTH)
			m_TransformCompPtr->SetPosition(0 - shape.width, shape.bottom);

	GetOwner()->GetComponent<Render>()->SetRect(shape);

	//std::cout << "AI: " << shape.left << " " << shape.bottom << std::endl;
}

void diji::AI::OnNotify(MessageTypes message, [[maybe_unused]] Subject* subject)
{
	auto msg = static_cast<MessageTypesDerived>(message);
	switch (msg)
	{
	case MessageTypesDerived::LEVEL_COLLISION:
	{
		std::cout << "AI: Level Collision" << std::endl;
		break;
	}
	case MessageTypesDerived::PICKUP_COLLISION:
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

const diji::Rectf diji::AI::CalculateNewPosition(Movement movement)
{
	auto shape = m_ColliderCompPtr->GetCollisionBox();
	//const auto& deltaTime = TimeSingleton::GetInstance().GetDeltaTime();
	switch (movement)
	{
	case Movement::Up:
		//shape.bottom -= m_Speed.y * deltaTime;
		//--shape.bottom;
		shape.bottom -= 2;
		break;
	case Movement::Down:
		//shape.bottom += m_Speed.y * deltaTime;
		//++shape.bottom;
		shape.bottom += 2;
		break;
	case Movement::Left:
		//shape.left -= m_Speed.x * deltaTime;
		//--shape.left;
		shape.left -= 2;
		break;
	case Movement::Right:
		//shape.left += m_Speed.x * deltaTime;
		//++shape.left;
		shape.left += 2;
		break;
	}

	if (Collision::GetInstance().IsCollidingWithWorld(shape))
	{
		switch (movement)
		{
		case Movement::Up:
			++shape.bottom;
			break;
		case Movement::Down:
			--shape.bottom;
			break;
		case Movement::Left:
			++shape.left;
			break;
		case Movement::Right:
			--shape.left;
			break;
		}
	}

	return shape;
}
