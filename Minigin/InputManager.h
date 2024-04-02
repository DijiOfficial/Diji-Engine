#pragma once
#include "Singleton.h"
#include "Controller.h"
#include "GameActorCommands.h"

#include <map>
#include <variant>

namespace diji 
{
	enum class KeyState
	{
		PRESSED,
		RELEASED,
		HELD,
	};
	enum class PlayerIdx
	{
		KEYBOARD = -1,
		PLAYER1,
		PLAYER2,
		PLAYER3,
		PLAYER4,
	};
	class Input final
	{
	public:
		typedef std::variant<SDL_Scancode, Controller::Button> InputType;
		
		Input(InputType input)
			: m_Input{ input }
		{
		}
		~Input() = default;

		Input(const Input& other) = default;
		Input(Input&& other) noexcept = default;
		Input& operator=(const Input& other) = delete;
		Input& operator=(Input&& other) noexcept = delete;

		InputType GetInput() const { return m_Input; }

	private:
		InputType m_Input;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		
		template<typename T, typename... Args>
			requires std::derived_from<T, GameActorCommands>
		void BindCommand(PlayerIdx playerIdx, KeyState state, typename Input::InputType input, GameObject* actor, Args... args)
		{
			if (playerIdx != PlayerIdx::KEYBOARD)
			{
				BindController(static_cast<int>(playerIdx));
			}
			m_CommandsUPtrMap.emplace(state, std::make_pair(Input(input), PlayerCommand{ playerIdx, std::make_unique<T>(actor, std::forward<Args>(args)...) }));
		}
		
	private:
		struct PlayerCommand
		{
			PlayerIdx playerIndex;
			std::unique_ptr<GameActorCommands> commandUPtr;
		};
		std::unordered_multimap<KeyState, std::pair<Input, PlayerCommand>> m_CommandsUPtrMap;
		
		std::map<int, std::unique_ptr<Controller>> m_PlayersMap;
		std::vector<int> m_ControllersIdxs;

		void ProcessControllerInput();
		void ProcessKeyboardInput();
		void BindController(int controllerIdx);
	};
}
