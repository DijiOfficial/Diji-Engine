#include <SDL.h>
#include "InputManager.h"
#include "GUI.h"
#include <cassert>
#include "ScoreCounter.h"

bool diji::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		
		switch (e.type)
		{
		case SDL_QUIT:
			return false; 

		case SDL_KEYDOWN:
			if (not m_KeyboardMoveUPtr) break;
			if (e.key.keysym.sym == SDLK_w)
				m_KeyboardMoveUPtr->KeyPressed(Movement::Up);
			else if (e.key.keysym.sym == SDLK_s)
				m_KeyboardMoveUPtr->KeyPressed(Movement::Down);
			else if (e.key.keysym.sym == SDLK_a)
				m_KeyboardMoveUPtr->KeyPressed(Movement::Left);
			else if (e.key.keysym.sym == SDLK_d)
				m_KeyboardMoveUPtr->KeyPressed(Movement::Right);
			

			break;
		case SDL_KEYUP:
			if (not m_KeyboardMoveUPtr) break;
			if (e.key.keysym.sym == SDLK_w)
				m_KeyboardMoveUPtr->KeyReleased(Movement::Up);
			else if (e.key.keysym.sym == SDLK_s)
				m_KeyboardMoveUPtr->KeyReleased(Movement::Down);
			else if (e.key.keysym.sym == SDLK_a)
				m_KeyboardMoveUPtr->KeyReleased(Movement::Left);
			else if (e.key.keysym.sym == SDLK_d)
				m_KeyboardMoveUPtr->KeyReleased(Movement::Right);
			else if (e.key.keysym.sym == SDLK_c)
				m_KeyboardHitUPtr->KeyReleased();
			else if (e.key.keysym.sym == SDLK_z)
				m_KeyboardScoreUPtr->KeyReleased(PointType::Enemy);
			else if (e.key.keysym.sym == SDLK_x)
				m_KeyboardScoreUPtr->KeyReleased(PointType::PickUp);

			break;
		default:
			break;
		}

		GUI::GetInstance().ProcessEvent(&e);
	}

	if (not m_ControllersIdxs.empty())
	{
		ProcessControllerInput();
	}

	return true;
}

void diji::InputManager::BindKeyboard(const GameObject* actor)
{
	m_KeyboardMoveUPtr = std::make_unique<MoveCommand>(actor);
	//add new commands
	m_KeyboardHitUPtr = std::make_unique<HitCommand>(actor);
	m_KeyboardScoreUPtr = std::make_unique<ScoreCommand>(actor);
}

void diji::InputManager::BindController(const GameObject* actor, int controllerIdx)
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

	m_ControllerMoveUPtr = std::make_unique<MoveCommand>(actor);
	//add new commands
	m_ControllerHitUPtr = std::make_unique<HitCommand>(actor);
	m_ControllerScoreUPtr = std::make_unique<ScoreCommand>(actor);
}

void diji::InputManager::ExecuteCommand()
{
	//need to template this, this will get old fast
	if (m_KeyboardMoveUPtr) 
		m_KeyboardMoveUPtr->Execute();
	if (m_KeyboardHitUPtr)
		m_KeyboardHitUPtr->Execute();


	if (m_ControllerMoveUPtr) 
		m_ControllerMoveUPtr->Execute();

	if (m_ControllerHitUPtr)
		m_ControllerHitUPtr->Execute();
}

void diji::InputManager::ProcessControllerInput()
{
	for (const int index : m_ControllersIdxs)
	{
		m_PlayersMap[index]->ProcessControllerInput();

		if (m_PlayersMap[index]->IsKeyDownThisFrame(Controller::Button::DPadUp))
			m_ControllerMoveUPtr->KeyPressed(Movement::Up);

		if (m_PlayersMap[index]->IsKeyUpThisFrame(Controller::Button::DPadUp))
			m_ControllerMoveUPtr->KeyReleased(Movement::Up);


		if (m_PlayersMap[index]->IsKeyDownThisFrame(Controller::Button::DPadDown))
			m_ControllerMoveUPtr->KeyPressed(Movement::Down);

		if (m_PlayersMap[index]->IsKeyUpThisFrame(Controller::Button::DPadDown))
			m_ControllerMoveUPtr->KeyReleased(Movement::Down);


		if (m_PlayersMap[index]->IsKeyDownThisFrame(Controller::Button::DPadLeft))
			m_ControllerMoveUPtr->KeyPressed(Movement::Left);

		if (m_PlayersMap[index]->IsKeyUpThisFrame(Controller::Button::DPadLeft))
			m_ControllerMoveUPtr->KeyReleased(Movement::Left);


		if (m_PlayersMap[index]->IsKeyDownThisFrame(Controller::Button::DPadRight))
			m_ControllerMoveUPtr->KeyPressed(Movement::Right);

		if (m_PlayersMap[index]->IsKeyUpThisFrame(Controller::Button::DPadRight))
			m_ControllerMoveUPtr->KeyReleased(Movement::Right);

		if (m_PlayersMap[index]->IsKeyUpThisFrame(Controller::Button::X))
			m_ControllerHitUPtr->KeyReleased();

		if (m_PlayersMap[index]->IsKeyDownThisFrame(Controller::Button::A))
			m_ControllerScoreUPtr->KeyReleased(PointType::Enemy);

		if (m_PlayersMap[index]->IsKeyDownThisFrame(Controller::Button::B))
			m_ControllerScoreUPtr->KeyReleased(PointType::PickUp);
	}
}
