#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace diji
{
	enum class Movement;

	class Transform final : public Component
	{
	public:
		Transform(GameObject* ownerPtr) : Component(ownerPtr) { SetPosition(0, 0, 0); };
		Transform(GameObject* ownerPtr, const float x, const float y, const float z) : Component(ownerPtr) { SetPosition(x, y, z); };
		Transform(GameObject* ownerPtr, const float x, const float y) : Component(ownerPtr) { SetPosition(x, y); };
		Transform(GameObject* ownerPtr, int x, int y, int z) : Component(ownerPtr) { SetPosition(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)); };
		Transform(GameObject* ownerPtr, int x, int y) : Component(ownerPtr) { SetPosition(static_cast<float>(x), static_cast<float>(y)); };
		~Transform() noexcept override = default;


		Transform(const Transform& other) = delete;
		Transform(Transform&& other) = delete;
		Transform& operator=(const Transform& other) = delete;
		Transform& operator=(Transform&& other) = delete;

		void Init() override;
		void Update() override {};
		void FixedUpdate() override {};

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const float x, const float y, const float z);
		void SetPosition(const float x, const float y);
		void SetPosition(glm::vec3 pos) { m_Position = pos; };
		void SetMovement(const Movement& movement) { m_CurrentMovement = movement; };
		Movement GetMovement() const { return m_CurrentMovement; };
		glm::vec3 GetMovementVector(const int value) const;

	private:
		glm::vec3 m_Position;
		Movement m_CurrentMovement;
	};
}
