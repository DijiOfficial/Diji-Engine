#include <SDL.h>
#include "InputManager.h"
#include "GUI.h"
#include <windows.h>

#include <iostream>
bool diji::InputManager::ProcessInput()
{
	//for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	//{
	//	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	//	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

	//	dwResult = XInputGetState(i, &m_CurrentState);

	//	if (dwResult == ERROR_SUCCESS)
	//	{
	//		m_ControllerIndex = i;
	//		// Controller is connected
	//	}
	//	else
	//	{
	//		// Controller is not connected
	//	}
	//}
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		
		switch (e.type)
		{
		case SDL_QUIT:
			return false; 

		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_w)
				m_IsGoingUp = true;
			else if (e.key.keysym.sym == SDLK_s)
				m_IsGoingDown = true;
			else if (e.key.keysym.sym == SDLK_a)
				m_IsGoingLeft = true;
			else if (e.key.keysym.sym == SDLK_d)
				m_IsGoingRight = true;

			break;
		case SDL_KEYUP:
			if (e.key.keysym.sym == SDLK_w)
				m_IsGoingUp = false;
			else if (e.key.keysym.sym == SDLK_s)
				m_IsGoingDown = false;
			else if (e.key.keysym.sym == SDLK_a)
				m_IsGoingLeft = false;
			else if (e.key.keysym.sym == SDLK_d)
				m_IsGoingRight = false;
			
			break;
		default:
			break;
		}

		GUI::GetInstance().ProcessEvent(&e);
	}

	return true;
}
