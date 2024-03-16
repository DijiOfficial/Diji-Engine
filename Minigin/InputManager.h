#pragma once
#include "Singleton.h"
#include <XInput.h>

namespace diji 
{
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

	
	private:
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
