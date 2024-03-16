#include "PlayerControl.h"
#include "Transform.h"
#include "Time.h"
#include "InputManager.h"

diji::PlayerControl::PlayerControl(GameObject* ownerPtr)
	: Component(ownerPtr)
{
	m_TransformComponentPtr = ownerPtr->GetComponent<Transform>();

	assert(ownerPtr->GetComponent<Transform>() and "Transform Component needs to be initialized before PlayerControl Component");
}

void diji::PlayerControl::Update()
{
	const auto& input = InputManager::GetInstance();
	const auto& deltaTime = Time::GetInstance().GetDeltaTime();
	auto pos = m_TransformComponentPtr->GetPosition();

	//only one opposite direction can be true at a time// might change later to not move if opposite directions are pressed over prefering one
	if (input.IsGoingUp() or input.IsPressed(XINPUT_GAMEPAD_DPAD_UP))
		pos.y -= m_Speed.y * deltaTime;
	else if (input.IsGoingDown() or input.IsPressed(XINPUT_GAMEPAD_DPAD_DOWN))
		pos.y += m_Speed.y * deltaTime;

	if (input.IsGoingRight() or input.IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT))
		pos.x += m_Speed.x * deltaTime;
	else if (input.IsGoingLeft() or input.IsPressed(XINPUT_GAMEPAD_DPAD_LEFT))
		pos.x -= m_Speed.x * deltaTime;

	m_TransformComponentPtr->SetPosition(pos);
}
