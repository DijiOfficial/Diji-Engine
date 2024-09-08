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
#include "ISoundSystem.h"
#include "GameLoader.h"

pacman::Move::Move(diji::GameObject* actor, diji::Movement movement)
	: GameActorCommands{ actor }
	, m_Movement{ movement }
{
	m_TransformComponentPtr = GetGameActor()->GetComponent<diji::Transform>();
	assert(m_TransformComponentPtr and "Move Command need to be initialized after GameObject Transform Component");
}

void pacman::Move::Execute()
{
	if (diji::SceneManager::GetInstance().GetActiveSceneId() != static_cast<int>(GameState::INTRO) and diji::SceneManager::GetInstance().GetActiveSceneId() != static_cast<int>(GameState::MENU) and diji::SceneManager::GetInstance().GetActiveSceneId() != static_cast<int>(GameState::GAMEOVER))
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

pacman::SkipLevel::SkipLevel(diji::GameObject* actor)
	: GameActorCommands{ actor }
{
	m_PelletCounterPtr = GetGameActor()->GetComponent<PelletCounter>();
}

void pacman::SkipLevel::Execute()
{
	m_PelletCounterPtr->SkipLevel();
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
	if (diji::SceneManager::GetInstance().GetActiveSceneId() != static_cast<int>(GameState::INTRO) and diji::SceneManager::GetInstance().GetActiveSceneId() != static_cast<int>(GameState::MENU) and diji::SceneManager::GetInstance().GetActiveSceneId() != static_cast<int>(GameState::GAMEOVER))
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
	case MenuButtons::Up:
		m_MenuComponentPtr->SwitchUp();
			break;
	case MenuButtons::Down:
		m_MenuComponentPtr->SwitchDown();
		break;
	case MenuButtons::Enter:
		m_MenuComponentPtr->ValidateChoice();
		break;

	default:
		break;
	}
}

pacman::MuteCommand::MuteCommand(diji::GameObject* actor)
	: GameActorCommands{ actor }
{

}

void pacman::MuteCommand::Execute()
{
	if (m_IsMuted)
		diji::ServiceLocator::GetSoundSystem().Resume();
	else
		diji::ServiceLocator::GetSoundSystem().Pause();

	m_IsMuted = !m_IsMuted;
}

pacman::SingleCommands::SingleCommands(diji::GameObject* actor, SingleCommandOption option)
	: GameActorCommands{ actor }
	, m_Option{ option }
{

}

void pacman::SingleCommands::Execute()
{
	if (diji::SceneManager::GetInstance().GetActiveSceneId() != static_cast<int>(GameState::INTRO))
		return;

	switch (m_Option)
	{
	case SingleCommandOption::SKIP_INTRO:
		Loader::PacmanMenu();
		diji::SceneManager::GetInstance().SetNextSceneToActivate(static_cast<int>(GameState::MENU));
		break;
	default:
		break;
	}
}
