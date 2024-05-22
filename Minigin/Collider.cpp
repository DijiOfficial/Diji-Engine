#include "Collider.h"
#include "Transform.h"
#include "GameObject.h"

diji::Collider::Collider(GameObject* ownerPtr, const int width, const int height)
	: Component(ownerPtr)
{
	m_TransformCompPtr = nullptr;
	m_CollisionBox = { 0, 0, static_cast<float>(width * 2), static_cast<float>(height * 2) };

}

diji::Collider::Collider(GameObject* ownerPtr, const int width, const int height, const glm::vec2& offset)
	: Component(ownerPtr)
	, m_IsOffsetSet{ true }
	, m_Offset{ offset }
{
	m_TransformCompPtr = nullptr;
	m_CollisionBox = { 0, 0, static_cast<float>(width * 2), static_cast<float>(height * 2) };
}

diji::Collider::Collider(GameObject* ownerPtr, const float width, const float height)
	: Component(ownerPtr)
{
	m_TransformCompPtr = nullptr;
	m_CollisionBox = { 0, 0, width * 2, height * 2 };
}

void diji::Collider::Init()
{
	m_TransformCompPtr = GetOwner()->GetComponent<Transform>();

	const auto& pos = m_TransformCompPtr->GetPosition();
	m_CollisionBox.left = pos.x + m_Offset.x;
	m_CollisionBox.bottom = pos.y + m_Offset.y;

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