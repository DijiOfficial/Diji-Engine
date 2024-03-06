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