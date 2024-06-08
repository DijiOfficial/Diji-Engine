#include "CustomCommands.h"

#include "Transform.h"
#include "HealthCounter.h"
#include "ScoreCounter.h"
#include "GameObject.h"
#include "EnterName.h"
#include "SceneManager.h"
#include "GameState.h"
#include "GhostAI.h"
#include "PelletCounter.h"
#include "Menu.h"
pacman::Move::Move(diji::GameObject* actor, diji::Movement movement)
	: GameActorCommands{ actor }
	, m_Movement{ movement }
{
	m_TransformComponentPtr = GetGameActor()->GetComponent<diji::Transform>();
	assert(m_TransformComponentPtr and "Move Command need to be initialized after GameObject Transform Component");
}

void pacman::Move::Execute()
{
	if (diji::SceneManager::GetInstance().GetActiveSceneId() != static_cast<int>(GameState::MENU) and diji::SceneManager::GetInstance().GetActiveSceneId() != static_cast<int>(GameState::GAMEOVER))
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

pacman::test::test(diji::GameObject* actor)
	: GameActorCommands{ actor }
{
	tester = GetGameActor()->GetComponent<PelletCounter>();
}

void pacman::test::Execute()
{
	tester->test();
}

pacman::GhostSwitchState::GhostSwitchState(diji::GameObject* actor, diji::Movement movement)
	: GameActorCommands{ actor }
	, m_Movement{ movement }
{
	m_GhostAIComp = GetGameActor()->GetComponent<pacman::GhostAI>();
	assert(m_GhostAIComp and "Move Command need to be initialized after GameObject Transform Component");
}

void pacman::GhostSwitchState::Execute()
{
	if (diji::SceneManager::GetInstance().GetActiveSceneId() != static_cast<int>(GameState::MENU) and diji::SceneManager::GetInstance().GetActiveSceneId() != static_cast<int>(GameState::GAMEOVER))
		m_GhostAIComp->SetNextMovement(m_Movement);
}

pacman::MenuSwitch::MenuSwitch(diji::GameObject* actor, MenuButtons button)
	: GameActorCommands{ actor }
	, m_Button{ button }
{
	m_MenuComponentPtr = GetGameActor()->GetComponent<pacman::Menu>();
}

void pacman::MenuSwitch::Execute()
{
	if (diji::SceneManager::GetInstance().GetActiveSceneId() != static_cast<int>(GameState::MENU))
		return;

	switch (m_Button)
	{
	case MenuButtons::Right:
		m_MenuComponentPtr->SwitchUp();
			break;
	case MenuButtons::Left:
		m_MenuComponentPtr->SwitchDown();
		break;
	case MenuButtons::Enter:
		m_MenuComponentPtr->ValidateChoice();
		break;

	default:
		break;
	}
}
