#include "CustomCommands.h"

#include "Transform.h"
#include "HealthCounter.h"
#include "ScoreCounter.h"
#include "GameObject.h"
#include "EnterName.h"
#include "SceneManager.h"
#include "GameState.h"
pacman::Move::Move(diji::GameObject* actor, diji::Movement movement)
	: GameActorCommands{ actor }
	, m_Movement{ movement }
{
	m_TransformComponentPtr = GetGameActor()->GetComponent<diji::Transform>();
	assert(m_TransformComponentPtr and "Move Command need to be initialized after GameObject Transform Component");
}

void pacman::Move::Execute()
{
	if (diji::SceneManager::GetInstance().GetActiveSceneId() == static_cast<int>(GameState::LEVEL)) //todo: add other valid states
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
    , m_ScoreComponentPtr{ actorPtr->GetComponent<ScoreCounter>() }
	, m_PointType{ point }
{
}

void pacman::ScoreCommand::Execute()
{
	m_ScoreComponentPtr->IncreaseScore(m_PointType);
}

pacman::NameChangeCommand::NameChangeCommand(diji::GameObject* actor, diji::Movement movement)
	: GameActorCommands{ actor }
	, m_Movement{ movement }
{
	m_NameComp = GetGameActor()->GetComponent<EnterName>();
}

void pacman::NameChangeCommand::Execute()
{
	if (diji::SceneManager::GetInstance().GetActiveSceneId() == static_cast<int>(GameState::GAMEOVER))
		m_NameComp->MoveLetter(m_Movement);
}
