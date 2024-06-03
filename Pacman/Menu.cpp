#include "Menu.h"
#include "GameObject.h"
#include "Render.h"
#include <SDL.h>
#include "SceneManager.h"
#include "GameState.h"

#include "GameLoader.h"
#include "SceneManager.h"

void pacman::Menu::Update()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (pStates[SDL_SCANCODE_RETURN])
	{
		GameState state = GameState::INVALID;
		switch (m_SelectedIndex)
		{
		case 0:
			state = GameState::LEVEL;
			//Loader::PacmanLevel();
			//diji::SceneManager::GetInstance().Init();
			break;
		case 1:
			state = GameState::COOP;
			break;
		case 2:
			state = GameState::VERSUS;
			break;
		}

		diji::SceneManager::GetInstance().SetNextSceneToActivate(static_cast<int>(state));
		return;
	}

	if (!pStates[SDL_SCANCODE_LEFT] && !pStates[SDL_SCANCODE_A])
		m_WasDownKey = false;

	if (!pStates[SDL_SCANCODE_RIGHT] && !pStates[SDL_SCANCODE_D])
		m_WasUpKey = false;

	if ((pStates[SDL_SCANCODE_LEFT] or pStates[SDL_SCANCODE_A]) && !m_WasDownKey)
	{
		--m_SelectedIndex;
		if (m_SelectedIndex < 0)
			m_SelectedIndex = 2;
		m_WasDownKey = true;
	}
	if ((pStates[SDL_SCANCODE_RIGHT] or pStates[SDL_SCANCODE_D]) && !m_WasUpKey)
	{
		m_SelectedIndex = (m_SelectedIndex + 1) % 3;
		m_WasUpKey = true;
	}

	//todo dirty flag
	if (m_SelectedIndex > 0)
		m_Shape.width = 60;
	else
		m_Shape.width = 30;

	m_Shape.left = 106.f + m_SelectedIndex * 75.f;
	if (m_SelectedIndex == 2)
		m_Shape.left += 30.f;

	GetOwner()->GetComponent<diji::Render>()->UpdateShape(m_Shape);
}
