#include "CustomCommands.h"

#include "Transform.h"
#include "HealthCounter.h"
#include "ScoreCounter.h"
#include "GameObject.h"

pacman::Move::Move(diji::GameObject* actor, diji::Movement movement)
	: GameActorCommands{ actor }
	, m_Movement{ movement }
{
	//todo: see if can add Init function
	m_TransformComponentPtr = GetGameActor()->GetComponent<diji::Transform>();
	assert(m_TransformComponentPtr and "Move Command need to be initialized after GameObject Transform Component");
}

void pacman::Move::Execute()
{
	m_TransformComponentPtr->SetMovement(m_Movement);
}

pacman::HitCommand::HitCommand(diji::GameObject* actor)
	: GameActorCommands{ actor }
{
	m_HealthComponentPtr = GetGameActor()->GetComponent<HealthCounter>();
}

void pacman::HitCommand::Execute()
{
	m_HealthComponentPtr->DecreaseHealth();
}

pacman::ScoreCommand::ScoreCommand(diji::GameObject* actorPtr, PointType point)
	: GameActorCommands{ actorPtr }
	, m_PointType{ point }
{
	m_ScoreComponentPtr = GetGameActor()->GetComponent<ScoreCounter>();
}

void pacman::ScoreCommand::Execute()
{
	m_ScoreComponentPtr->IncreaseScore(m_PointType);
}