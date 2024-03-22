#pragma once
#include "Singleton.h"
#include <windows.h>
#include <XInput.h>

#include "MoveCommand.h"

//#include <memory>


namespace diji 
{
	//check includes too tired rn
	class MoveCommand;
	class GameObject;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		//bool IsPressed(unsigned int button) const;
		
		//Going to implement key remapping later by loading controls from a file
		bool IsGoingUp() const { return m_IsGoingUp; };
		bool IsGoingDown() const { return m_IsGoingDown; };
		bool IsGoingLeft() const { return m_IsGoingLeft; };
		bool IsGoingRight() const { return m_IsGoingRight; };

		//Controller
		bool IsDownThisFrame(unsigned int button) const
		{
			return m_ButtonsPressedThisFrame & button;
		}
		bool IsUpThisFrame(unsigned int button) const
		{
			return m_ButtonsReleasedThisFrame & button;
		}
		bool IsPressed(unsigned int button) const
		{
			return m_CurrentState.Gamepad.wButtons & button;
		}

		void BindKeyboard(GameObject* actor); //const gameObj
		void BindController(GameObject* actor);

		void ExecuteCommand()
		{
			m_KeyboardMoveUPtr->Execute();
			//m_ControllerMoveUPtr->Execute();
		}
	
	private:
		//template this? or vector of all possible commands?
		std::unique_ptr<MoveCommand> m_KeyboardMoveUPtr{ nullptr };
		std::unique_ptr<MoveCommand> m_ControllerMoveUPtr{ nullptr };

		XINPUT_STATE m_PreviousState{}, m_CurrentState{};
		DWORD dwResult;
		int m_ControllerIndex = 0;
		unsigned int m_ButtonsPressedThisFrame;
		unsigned int m_ButtonsReleasedThisFrame;

		bool m_IsGoingUp;
		bool m_IsGoingDown;
		bool m_IsGoingLeft;
		bool m_IsGoingRight;
	};
}
