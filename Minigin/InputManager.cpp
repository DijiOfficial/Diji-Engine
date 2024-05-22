#include "InputManager.h"
#include "GUI.h"
#include <SDL.h>

#include <SDL_gamecontroller.h>
#include <format>
bool diji::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		
		switch (e.type)
		{
		case SDL_QUIT:
			return false; 

		case SDL_KEYDOWN:

			break;
		case SDL_KEYUP:
			for (const auto& pair : m_CommandsUPtrMap)
			{
				if (pair.first != KeyState::RELEASED)
					continue;

				const Input& input = pair.second.first;
				const auto& inputType = input.GetInput();

				// Check if inputType holds SDL_Scancode
				if (std::holds_alternative<SDL_Scancode>(inputType))
				{
					// Get the SDL_Scancode from the variant
					SDL_Scancode scancode = std::get<SDL_Scancode>(inputType);
					if (e.key.keysym.scancode == scancode)
					{
						pair.second.second.commandUPtr->Execute();
					}
				}
			}

			break;
		default:
			break;
		}

		GUI::GetInstance().ProcessEvent(&e);
	}
	
	ProcessKeyboardInput();

	if (not m_ControllersIdxs.empty())
	{
		ProcessControllerInput();
	}

	return true;
}

void diji::InputManager::ProcessKeyboardInput()
{
	auto pKeyboard = SDL_GetKeyboardState(nullptr);
	for (const auto& pair : m_CommandsUPtrMap)
	{
		if (pair.first != KeyState::HELD)
			continue;

		const Input& input = pair.second.first;
		const auto& inputType = input.GetInput();

		// Check if inputType holds SDL_Scancode
		if (std::holds_alternative<SDL_Scancode>(inputType))
		{
			// Get the SDL_Scancode from the variant
			SDL_Scancode scancode = std::get<SDL_Scancode>(inputType);
			if (pKeyboard[scancode])
			{
				pair.second.second.commandUPtr->Execute();
			}
		}
	}
}

void diji::InputManager::BindController(int controllerIdx)
{
	if (controllerIdx < 0 or controllerIdx > 3) 	
	{
		assert(std::format("Controller index {} is invalid. XInput support controller 0-3", controllerIdx).c_str());
	}

	if (std::find(m_ControllersIdxs.begin(), m_ControllersIdxs.end(), controllerIdx) == m_ControllersIdxs.end())
	{
		m_PlayersMap[controllerIdx] = std::make_unique<Controller>(controllerIdx);
		m_ControllersIdxs.push_back(controllerIdx);
	}
	else
	{
		assert(std::format("Controller with index {} already exists.", controllerIdx).c_str());
	}
}


void diji::InputManager::ProcessControllerInput()
{	
	for (const int index : m_ControllersIdxs)
	{
		m_PlayersMap[index]->ProcessControllerInput();
		
		for (const auto& pair : m_CommandsUPtrMap)
		{
			if (static_cast<int>(pair.second.second.playerIndex) != index) 
				continue;

			const Input& input = pair.second.first;
			const auto& inputType = input.GetInput();

			// Check if inputType holds Controller::Button
			if (not std::holds_alternative<Controller::Button>(inputType))
				continue;

			// Get the SDL_Scancode from the variant
			Controller::Button scancode = std::get<Controller::Button>(inputType);
				
			switch (pair.first)
			{
				case KeyState::HELD:
					if (m_PlayersMap[index]->IsPressed(scancode))
						pair.second.second.commandUPtr->Execute();
					break;
				case KeyState::PRESSED:
					if (m_PlayersMap[index]->IsKeyDownThisFrame(scancode))
						pair.second.second.commandUPtr->Execute();
					break;
				case KeyState::RELEASED:
					if (m_PlayersMap[index]->IsKeyUpThisFrame(scancode))
						pair.second.second.commandUPtr->Execute();
					break;
			default:
				break;
			}				
		}
	}
}