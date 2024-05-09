#include "AI.h"
#include "Texture.h"
#include "Transform.h"
#include "Collider.h"
#include "PickUp.h"
#include "ScoreCounter.h"

#include "Observers.h"

diji::AI::AI(GameObject* ownerPtr)
	: Component(ownerPtr)
{
	m_TextureCompPtr = ownerPtr->GetComponent<Texture>();
	m_TransformCompPtr = ownerPtr->GetComponent<Transform>();
	m_ColliderCompPtr = ownerPtr->GetComponent<Collider>();
	assert(m_TextureCompPtr and "AI Component needs to be initialized aftera Texture");
	assert(m_TransformCompPtr and "AI Component needs to be initialized aftera Transform");
	assert(m_ColliderCompPtr and "AI Component needs to be initialized aftera Collider");
}

void diji::AI::Update()
{
	const auto& currentMovement = m_TransformCompPtr->GetMovement();
	if (m_PreviousMovement != currentMovement)
	{
		m_PreviousMovement = currentMovement;
		switch (currentMovement)
		{
		case Movement::Right:
			m_TextureCompPtr->SetRotationAngle(0.f);
			break;
		case Movement::Down:
			m_TextureCompPtr->SetRotationAngle(90.f);
			break;
		case Movement::Left:
			m_TextureCompPtr->SetRotationAngle(180.f);
			break;
		case Movement::Up:
			m_TextureCompPtr->SetRotationAngle(270.f);
			break;
		case Movement::Idle:
		default:
			break;
		}
	}
}

void diji::AI::OnNotify(MessageTypes message, [[maybe_unused]] Subject* subject)
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
