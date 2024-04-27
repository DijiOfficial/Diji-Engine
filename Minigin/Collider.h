#pragma once
#include "Component.h"
#include "Subject.h"
#include "Collision.h"

namespace diji 
{
	class Transform;
	class Collider : public Component, public Subject
	{
	public:
		Collider(GameObject* ownerPtr, const float width, const float height);
		Collider(GameObject* ownerPtr, const int width, const int height);
		~Collider() override = default;

		Collider(const Collider& other) = delete;
		Collider(Collider&& other) = delete;
		Collider& operator=(const Collider& other) = delete;
		Collider& operator=(Collider&& other) = delete;

		void Update() override;
		void NotifyWorldCollision();
		void NotifyCollision();
		Rectf GetCollisionBox() const { return m_CollisionBox; }
	private:
		Rectf m_CollisionBox;
		Transform* m_TransformCompPtr;
	};
}

