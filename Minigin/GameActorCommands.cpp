#include "GameActorCommands.h"

#include "TimeSingleton.h"
#include "Transform.h"
#include "HealthCounter.h"
#include "ScoreCounter.h"
#include "Collider.h"

diji::Move::Move(GameObject* actor, Movement movement)
	: GameActorCommands{ actor }
	, m_Movement{ movement }
{
	m_TransformComponentPtr = GetGameActor()->GetComponent<Transform>();
	m_CollisionComponentPtr = GetGameActor()->GetComponent<Collider>();

	assert(m_TransformComponentPtr and "Move Command need to be initialized after GameObject Transform Component");
	assert(m_CollisionComponentPtr and "Move Command need to be initialized after GameObject Collision Component");
}

void diji::Move::Execute()
{
	const auto& deltaTime = TimeSingleton::GetInstance().GetDeltaTime();
	auto pos = m_TransformComponentPtr->GetPosition();

	switch (m_Movement)
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

	m_TransformComponentPtr->SetMovement(m_Movement);

	auto shape = m_CollisionComponentPtr->GetCollisionBox();
	shape.left = pos.x;
	shape.bottom = pos.y;

	if (not Collision::GetInstance().IsCollidingWithWorld(shape))
		m_TransformComponentPtr->SetPosition(pos);
}

diji::HitCommand::HitCommand(GameObject* actor)
	: GameActorCommands{ actor }
{
	m_HealthComponentPtr = GetGameActor()->GetComponent<HealthCounter>();
}

void diji::HitCommand::Execute()
{
	m_HealthComponentPtr->DecreaseHealth();
}

diji::ScoreCommand::ScoreCommand(GameObject* actorPtr, PointType point)
	: GameActorCommands{ actorPtr }
	, m_PointType{ point }
{
	m_ScoreComponentPtr = GetGameActor()->GetComponent<ScoreCounter>();
}

void diji::ScoreCommand::Execute()
{
	m_ScoreComponentPtr->IncreaseScore(m_PointType);
}