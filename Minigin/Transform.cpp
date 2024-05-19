#include "Transform.h"
#include "Render.h"

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

void diji::Transform::CheckRenderComponent() 
{
	assert(!GetOwner()->HasComponent<Render>() and "Transform Component needs to be initialized before Render");
}

glm::vec3 diji::Transform::GetMovementVector(const int value) const
{
	switch (m_CurrentMovement)
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