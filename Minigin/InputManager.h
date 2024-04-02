#pragma once
#include "Singleton.h"
#include "Controller.h"
//#include "MoveCommand.h"

#include "GameActorCommands.h"
#include <map>
#include <variant>

namespace diji 
{
	enum class ActionType
	{
		MoveUp,
		MoveDown,
		MoveLeft,
		MoveRight,
		TakeDamage,
	};
	enum class KeyState
	{
		PRESSED,
		RELEASED,
		HELD,
	};
	class Input final
	{
	public:
		typedef std::variant<SDL_Scancode, SDL_GameControllerButton> InputType;
		

		//Input(InputType input, State state)
		Input(InputType input)
			: m_Input{ input }
			//, m_State{ state }
		{
		}
		~Input() = default;
		InputType GetInput() const { return m_Input; }

	private:
		InputType m_Input;
		//enum State m_State;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		
		//void BindKeyboard(const GameObject* actor);
		//void BindController(const GameObject* actorx);
		void BindController(const GameObject* actor, int controllerIdx);

		void ExecuteCommand();

		template<typename T, typename... Args>
			requires std::derived_from<T, GameActorCommands>
		void BindKeyboard(KeyState state, typename Input::InputType input, GameObject* actor, Args... args)
		{
			m_CommandsUPtrMap.emplace(state, std::make_pair(Input(input), std::make_unique<T>(actor, std::forward<Args>(args)...)));
		}
		
	private:

		std::unordered_multimap<KeyState, std::pair<Input, std::unique_ptr<GameActorCommands>>> m_CommandsUPtrMap;

		//template this? or vector of all possible commands?
		//std::unique_ptr<MoveCommand> m_KeyboardMoveUPtr{ nullptr };
		//std::unique_ptr<HitCommand> m_KeyboardHitUPtr{ nullptr };
		//std::unique_ptr<ScoreCommand> m_KeyboardScoreUPtr{ nullptr };

		//std::unique_ptr<MoveCommand> m_ControllerMoveUPtr{ nullptr };
		//std::unique_ptr<HitCommand> m_ControllerHitUPtr{ nullptr };
		//std::unique_ptr<ScoreCommand> m_ControllerScoreUPtr{ nullptr };
		

		std::map<int, std::unique_ptr<Controller>> m_PlayersMap;
		std::vector<int> m_ControllersIdxs;
		void ProcessControllerInput();
		void ProcessKeyboardInput();
	};
}
