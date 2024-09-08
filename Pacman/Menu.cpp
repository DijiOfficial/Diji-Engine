#include "Menu.h"
#include "GameObject.h"
#include "Render.h"
#include <SDL.h>
#include "SceneManager.h"
#include "GameState.h"

#include "GameLoader.h"
#include "SceneManager.h"
#include "TimeSingleton.h"
#include "InputManager.h"

void pacman::Menu::Init()
{
	const auto& owner = GetOwner();
	const auto& back = owner->GetChild(0);
	//m_BackRenderCompPtr = back->GetComponent<diji::Render>();
	m_BackTransformCompPtr = back->GetComponent<diji::Transform>();

	for (int i = 1; i < 5; ++i)
	{
		m_MultiplayerRenderCompPtrVec.push_back(owner->GetChild(i)->GetComponent<diji::Render>());
	}
	m_MultiplayerRenderCompPtrVec.push_back(owner->GetChild(0)->GetComponent<diji::Render>());

	for (int i = 5; i < 11; ++i)
	{
		m_TextRenderCompPtrVec.push_back(owner->GetChild(i)->GetComponent<diji::Render>());
	}
}

void pacman::Menu::Update()
{
    m_DisplayTimer += diji::TimeSingleton::GetInstance().GetDeltaTime();
    if (m_DisplayTimer > 0.2f)
    {
        m_Display = !m_Display;
        m_DisplayTimer -= 0.2f;

        const std::vector<diji::Render*>* renderCompPtrVec = GetCurrentStateRenderCompPtrVec();

		const int displayIndex = GetDisplayIndex();

        if (m_Display)
            (*renderCompPtrVec)[displayIndex]->EnableRender();
        else
            (*renderCompPtrVec)[displayIndex]->DisableRender();
    }
}

void pacman::Menu::ValidateChoice()
{
	const float backTextYOffset = 344.f;
	GameState state = GameState::INVALID;
	switch (m_MenuState)
	{
	case pacman::MenuState::MENU:
		switch (m_SelectedIndex)
		{
		case 0:
			state = GameState::LEVEL;
			Loader::PacmanLevel();
			break;
		case 1:
			for (const auto& renderComp : m_TextRenderCompPtrVec)
			{
				renderComp->DisableRender();
			}

			m_MultiplayerRenderCompPtrVec[0]->EnableRender();
			m_MultiplayerRenderCompPtrVec[1]->EnableRender();
			m_MultiplayerRenderCompPtrVec[4]->EnableRender();

			m_BackTransformCompPtr->SetPosition(m_BackTransformCompPtr->GetPosition().x, backTextYOffset - 20.f);
			
			m_MenuState = MenuState::MULTIPLAYER;
			break;
		case 2: //create level
		case 3://options
		case 4://highscores
			break;
		case 5:
			diji::InputManager::GetInstance().Quit();
			break;
		}
		break;
	case pacman::MenuState::MULTIPLAYER:
		switch (m_SelectedIndex)
		{
		case 0:
			state = GameState::COOP;
			Loader::CoopLevel();
			break;
		case 1:
			m_MultiplayerRenderCompPtrVec[0]->DisableRender();
			m_MultiplayerRenderCompPtrVec[1]->DisableRender();

			m_MultiplayerRenderCompPtrVec[2]->EnableRender();
			m_MultiplayerRenderCompPtrVec[3]->EnableRender();
			m_MultiplayerRenderCompPtrVec[4]->EnableRender();

			m_MenuState = MenuState::MULTIPLAYEROPTIONS;

			break;
		case 2:
			m_MultiplayerRenderCompPtrVec[0]->DisableRender();
			m_MultiplayerRenderCompPtrVec[1]->DisableRender();
			m_MultiplayerRenderCompPtrVec[4]->DisableRender();
			for (const auto& renderComp : m_TextRenderCompPtrVec)
			{
				renderComp->EnableRender();
			}
			m_MenuState = MenuState::MENU;
			break;
		}
		break;
	case pacman::MenuState::MULTIPLAYEROPTIONS:
		switch (m_SelectedIndex)
		{
		case 0://Freemode versus
			//state = GameState::VERSUS;
			//Loader::CoopLevel();
			break;
		case 1:
			state = GameState::VERSUS;
			Loader::VersusLevel();
			break;
		case 2:
			m_MultiplayerRenderCompPtrVec[0]->EnableRender();
			m_MultiplayerRenderCompPtrVec[1]->EnableRender();

			m_MultiplayerRenderCompPtrVec[2]->DisableRender();
			m_MultiplayerRenderCompPtrVec[3]->DisableRender();
			m_MultiplayerRenderCompPtrVec[4]->EnableRender();
			
			m_MenuState = MenuState::MULTIPLAYER;
			break;
		}
		break;
	case pacman::MenuState::OPTIONS:
		break;
	case pacman::MenuState::HIGHSCORE:
		break;
	default:
		break;
	}

	m_SelectedIndex = 0;
	if (state != GameState::INVALID)
		diji::SceneManager::GetInstance().SetNextSceneToActivate(static_cast<int>(state));
}

void pacman::Menu::SwitchDown()
{
	(*GetCurrentStateRenderCompPtrVec())[GetDisplayIndex()]->EnableRender();
	int maxIndex = static_cast<int>(m_MenuState);
	if (m_MenuState == MenuState::MULTIPLAYEROPTIONS)
		maxIndex = 3;

	m_SelectedIndex = (m_SelectedIndex + 1) % maxIndex;
}

void pacman::Menu::SwitchUp()
{
	(*GetCurrentStateRenderCompPtrVec())[GetDisplayIndex()]->EnableRender();
	--m_SelectedIndex;
	if (m_SelectedIndex < 0)
	{
		int maxIndex = static_cast<int>(m_MenuState) - 1;
		if (m_MenuState == MenuState::MULTIPLAYEROPTIONS)
			maxIndex = 2;
		m_SelectedIndex = maxIndex;
	}
}

std::vector<diji::Render*>* pacman::Menu::GetCurrentStateRenderCompPtrVec()
{
	switch (m_MenuState)
	{
	case pacman::MenuState::MENU:
		return &m_TextRenderCompPtrVec;
		break;

	case pacman::MenuState::MULTIPLAYEROPTIONS:
	case pacman::MenuState::MULTIPLAYER:
		return &m_MultiplayerRenderCompPtrVec;
		break;
	case pacman::MenuState::OPTIONS:
	case pacman::MenuState::HIGHSCORE:
	default:
		return nullptr;
		//return std::vector<diji::Render*>;
	}
}

int pacman::Menu::GetDisplayIndex() const
{
	int displayIndex = m_SelectedIndex;
	if (m_MenuState == MenuState::MULTIPLAYEROPTIONS)
		displayIndex = m_SelectedIndex + 2;

	if ((m_MenuState == MenuState::MULTIPLAYER or m_MenuState == MenuState::MULTIPLAYEROPTIONS) and m_SelectedIndex == 2)
		displayIndex = 4;

	return displayIndex;
}
