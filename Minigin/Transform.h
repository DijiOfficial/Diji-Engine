#pragma once
#include <glm/glm.hpp>
#include "GameObject.h"

namespace diji
{
	class GameObject;

	class Transform final : public Component
	{
	public:
		Transform(GameObject* ownerPtr) : Component(ownerPtr) { SetPosition(0, 0, 0); };
		Transform(GameObject* ownerPtr, const float x, const float y, const float z) : Component(ownerPtr) { SetPosition(x, y, z); };
		Transform(GameObject* ownerPtr, const float x, const float y) : Component(ownerPtr) { SetPosition(x, y); };
		Transform(GameObject* ownerPtr, int x, int y, int z) : Component(ownerPtr) { SetPosition(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)); };
		Transform(GameObject* ownerPtr, int x, int y) : Component(ownerPtr) { SetPosition(static_cast<float>(x), static_cast<float>(y)); };
		virtual ~Transform() = default;


		Transform(const Transform& other) = delete;
		Transform(Transform&& other) = delete;
		Transform& operator=(const Transform& other) = delete;
		Transform& operator=(Transform&& other) = delete;

		virtual void Update() override { };

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const float x, const float y, const float z);
		void SetPosition(const float x, const float y);

	private:
		glm::vec3 m_Position;
	};
}
