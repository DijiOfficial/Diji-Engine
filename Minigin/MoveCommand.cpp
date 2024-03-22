#include "MoveCommand.h"
#include "Time.h"
#include "Transform.h"

diji::MoveCommand::MoveCommand(GameObject* actorPtr) 
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
