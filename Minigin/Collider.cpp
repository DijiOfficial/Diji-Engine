#include "Collider.h"
#include "Transform.h"

diji::Collider::Collider(GameObject* ownerPtr, const int width, const int height)
	: Component(ownerPtr)
{
	m_TransformCompPtr = ownerPtr->GetComponent<Transform>();
	assert(m_TransformCompPtr and "GameObject must have a Transform component to have a Collider component");

	const auto& pos = m_TransformCompPtr->GetPosition();
	m_CollisionBox = { pos.x, pos.y, static_cast<float>(width * 2), static_cast<float>(height * 2)};

	Collision::GetInstance().AddCollider(this, m_CollisionBox);
}

diji::Collider::Collider(GameObject* ownerPtr, const int width, const int height, const glm::vec2& offset)
	: Component(ownerPtr)
	, m_IsOffsetSet{ true }
	, m_Offset{ offset }
{
	m_TransformCompPtr = ownerPtr->GetComponent<Transform>();
	assert(m_TransformCompPtr and "GameObject must have a Transform component to have a Collider component");

	const auto& pos = m_TransformCompPtr->GetPosition();
	m_CollisionBox = { pos.x + offset.x, pos.y + offset.y, static_cast<float>(width), static_cast<float>(height) };

	Collision::GetInstance().AddCollider(this, m_CollisionBox);
}

diji::Collider::Collider(GameObject* ownerPtr, const float width, const float height)
	: Component(ownerPtr)
{
	m_TransformCompPtr = ownerPtr->GetComponent<Transform>();
	assert(m_TransformCompPtr and "GameObject must have a Transform component to have a Collider component");

	const auto& pos = m_TransformCompPtr->GetPosition();
	m_CollisionBox = { pos.x, pos.y, width * 2, height * 2};

	Collision::GetInstance().AddCollider(this, m_CollisionBox);
}

void diji::Collider::Update()
{
	const auto& pos = m_TransformCompPtr->GetPosition();
	if ((pos.x == m_CollisionBox.left and pos.y == m_CollisionBox.bottom) or m_IsOffsetSet)
		return;

	m_CollisionBox.left = pos.x;
	m_CollisionBox.bottom = pos.y;

	Collision::GetInstance().UpdateCollider(this, m_CollisionBox);
}