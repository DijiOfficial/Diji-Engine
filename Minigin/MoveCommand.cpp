#include "MoveCommand.h"
#include "Time.h"
#include "Transform.h"
#include "HealthCounter.h"
#include "ScoreCounter.h"

diji::MoveCommand::MoveCommand(const GameObject* actorPtr)
{
	m_TransformComponentPtr = actorPtr->GetComponent<Transform>();

	assert(actorPtr->GetComponent<Transform>() and "GameObjects needs to be initialized before PlayerControls");
}

void diji::MoveCommand::Execute()
{
	const auto& deltaTime = Time::GetInstance().GetDeltaTime();
	auto pos = m_TransformComponentPtr->GetPosition();

	for (const auto& movement : m_Movement)
	{
		if (movement.second)
		{
			switch (movement.first)
			{
			case Movement::Up:
				pos.y -= m_Speed.y * deltaTime;
				break;
			case Movement::Down:
				pos.y += m_Speed.y * deltaTime;
				break;
			case Movement::Left:
				pos.x -= m_Speed.x * deltaTime;
				break;
			case Movement::Right:
				pos.x += m_Speed.x * deltaTime;
				break;
			}
		}
	}

	m_TransformComponentPtr->SetPosition(pos);
}

//diji::HitCommand::HitCommand(const GameObject* actorPtr)
//{
//	m_HealthComponentPtr = actorPtr->GetComponent<HealthCounter>();
//
//	assert(actorPtr->GetComponent<HealthCounter>() and "GameObjects & HealthComp needs to be initialized before PlayerControls");
//
//}
//void diji::HitCommand::Execute()
//{
//	if (m_IsHit)
//	{
//		m_HealthComponentPtr->DecreaseHealth();
//		m_IsHit = false;
//	}
//}

diji::ScoreCommand::ScoreCommand(const GameObject* actorPtr)
{
	m_ScoreComponentPtr = actorPtr->GetComponent<ScoreCounter>();

	assert(m_ScoreComponentPtr and "GameObjects & ScoreComp needs to be initialized before PlayerControls");

}