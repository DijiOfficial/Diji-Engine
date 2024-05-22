#pragma once
#include "Component.h"
#include "Collision.h"

//todo: verify list iniutialize order
namespace diji 
{
	class Transform;
	class Collider : public Component
	{
	public:
		Collider(GameObject* ownerPtr, const float width, const float height);
		Collider(GameObject* ownerPtr, const int width, const int height);
		Collider(GameObject* ownerPtr, const int width, const int height, const glm::vec2& offset);
		~Collider() override = default;

		Collider(const Collider& other) = delete;
		Collider(Collider&& other) = delete;
		Collider& operator=(const Collider& other) = delete;
		Collider& operator=(Collider&& other) = delete;

		void Init() override;
		void Update() override;
		void FixedUpdate() override {};

		Rectf GetCollisionBox() const { return m_CollisionBox; };
		const GameObject* GetParent() const { return GetOwner(); };
	
	private:
		bool m_IsOffsetSet = false;
		glm::vec2 m_Offset{ 0, 0 };
		Transform* m_TransformCompPtr;
		Rectf m_CollisionBox;

	};
}

