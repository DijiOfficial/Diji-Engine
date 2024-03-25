#pragma once
#include "Singleton.h"
#include "Controller.h"
#include "MoveCommand.h"
#include <map>

namespace diji 
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		
		void BindKeyboard(const GameObject* actor);
		//void BindController(const GameObject* actorx);
		void BindController(const GameObject* actor, int controllerIdx);

		void ExecuteCommand();
	
	private:
		//template this? or vector of all possible commands?
		std::unique_ptr<MoveCommand> m_KeyboardMoveUPtr{ nullptr };
		std::unique_ptr<HitCommand> m_KeyboardHitUPtr{ nullptr };
		std::unique_ptr<ScoreCommand> m_KeyboardScoreUPtr{ nullptr };

		std::unique_ptr<MoveCommand> m_ControllerMoveUPtr{ nullptr };
		std::unique_ptr<HitCommand> m_ControllerHitUPtr{ nullptr };
		std::unique_ptr<ScoreCommand> m_ControllerScoreUPtr{ nullptr };
		//template this to allow creation of any controller very likely nedd 0-3 controllers because of Xinput only supports 0-3
		//std::unique_ptr<Controller> m_ControllerUPtr{ std::make_unique<Controller>(0) };
		//std::unique_ptr<Controller> m_ControllerUPtr{ nullptr };
		std::map<int, std::unique_ptr<Controller>> m_PlayersMap;
		std::vector<int> m_ControllersIdxs;
		void ProcessControllerInput();
	};
}
