#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <XInput.h>
#include "Controller.h"
#include <cassert>

class diji::Controller::XInput
{
	public:
		XInput(int controllerIdx) 
			: m_ControllerIdx{ controllerIdx }
		{
			//this does not work the way I intended
			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));
			DWORD dwResult = XInputGetState(controllerIdx, &state);

			if (dwResult != ERROR_SUCCESS)
			{
				assert("Controller not connected");
			}
		}

		XInput(const XInput& other) = delete;
		XInput(XInput&& other) = delete;
		XInput& operator=(const XInput& other) = delete;
		XInput& operator=(XInput&& other) = delete;

		void ProcessControllerInput()
		{
			CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
			ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
			XInputGetState(m_ControllerIdx, &m_CurrentState);

			auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
			m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
			m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
		}

		bool IsDownThisFrame(unsigned int button) { return m_ButtonsPressedThisFrame & button; }
		bool IsUpThisFrame(unsigned int button) { return m_ButtonsReleasedThisFrame & button; };

	private:
		int m_ControllerIdx{};
		XINPUT_STATE m_PreviousState{};
		XINPUT_STATE m_CurrentState{};
		unsigned int m_ButtonsPressedThisFrame{};
		unsigned int m_ButtonsReleasedThisFrame{};
};

diji::Controller::Controller(int controllerIdx) 
	: m_XInputUPtr{ std::make_unique<XInput>(controllerIdx) }
{
}

diji::Controller::~Controller()
{
}

void diji::Controller::ProcessControllerInput()
{
	m_XInputUPtr->ProcessControllerInput();
}

bool diji::Controller::IsKeyDownThisFrame(const Button& button)
{
	return m_XInputUPtr->IsDownThisFrame(static_cast<unsigned int>(button));
}

bool diji::Controller::IsKeyUpThisFrame(const Button& button)
{
	return m_XInputUPtr->IsUpThisFrame(static_cast<unsigned int>(button));
}