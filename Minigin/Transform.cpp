#include "Transform.h"
#include "Command.h"

void diji::Transform::Init()
{
	m_CurrentMovement = Movement::Idle;
	m_LastMovement = Movement::Idle;
	m_LookingDirection = Movement::Idle;
}

void diji::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void diji::Transform::SetPosition(const float x, const float y)
{
	SetPosition(x, y, 0.f);
}

glm::vec3 diji::Transform::GetMovementVector(const float value) const
{
	Movement movement = m_CurrentMovement == Movement::Idle ? m_LastMovement : m_CurrentMovement;
	if (m_LookingDirection != Movement::Idle)
		movement = m_LookingDirection;

	if (movement == Movement::Idle)
		return glm::vec3(0, 0, 0);

	switch (movement)
	{
	case Movement::Up:
		return glm::vec3(0, -value, 0);
	case Movement::Down:
		return glm::vec3(0, value, 0);
	case Movement::Left:
		return glm::vec3(-value, 0, 0);
	case Movement::Right:
		return glm::vec3(value, 0, 0);
	default:
		return glm::vec3(0, 0, 0);
	}
}

glm::vec2 diji::Transform::Get2DMovementVector(const float value) const
{
	Movement movement = m_CurrentMovement == Movement::Idle ? m_LastMovement : m_CurrentMovement;
	if (m_LookingDirection != Movement::Idle)
		movement = m_LookingDirection;

	if (movement == Movement::Idle)
		return glm::vec3(0, 0, 0);

	switch (movement)
	{
	case Movement::Up:
		return glm::vec2(0, -value);
	case Movement::Down:
		return glm::vec2(0, value);
	case Movement::Left:
		return glm::vec2(-value, 0);
	case Movement::Right:
		return glm::vec2(value, 0);
	default:
		return glm::vec2(0, 0);
	}
}

glm::vec2 diji::Transform::Get2DMovementVector(const Movement& movement, const float value) const
{
	switch (movement)
	{
	case Movement::Up:
		return glm::vec2(0, -value);
	case Movement::Down:
		return glm::vec2(0, value);
	case Movement::Left:
		return glm::vec2(-value, 0);
	case Movement::Right:
		return glm::vec2(value, 0);
	default:
		return glm::vec2(0, 0);
	}
}
