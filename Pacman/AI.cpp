#include "AI.h"
#include "Texture.h"
#include "Transform.h"

diji::AI::AI(GameObject* ownerPtr)
	: Component(ownerPtr)
{
	m_TextureCompPtr = ownerPtr->GetComponent<Texture>();
	m_TransformCompPtr = ownerPtr->GetComponent<Transform>();
	assert(m_TextureCompPtr and "AI Component needs to be initialized before Texture");
	assert(m_TransformCompPtr and "AI Component needs to be initialized before Transform");
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

#include <iostream>
void diji::AI::OnNotify(MessageTypes message, Subject* subject)
{
	(void)subject;
	switch (message)
	{
	case MessageTypes::LEVEL_COLLISION:
	{

		//HealthCounter* healthCounter = dynamic_cast<HealthCounter*>(subject);
		//if (not healthCounter)
		//	return; // throw error instead

		//const int lives = healthCounter->GetHealth();
		//std::string text = "lives";
		//if (lives == 1)
		//{
		//	text = "life";
		//}
		//SetText(std::format("# {}: {}", text, lives));
		std::cout << "AI: Level Collision" << std::endl;
		break;
	}
	default:
		break;
	}
}
