#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace diji
{
	enum class Movement;

	class Transform final : public Component
	{
	public:
		explicit Transform(GameObject* ownerPtr) : Component(ownerPtr) { SetPosition(0, 0, 0); };
		explicit Transform(GameObject* ownerPtr, const float x, const float y, const float z) : Component(ownerPtr) { SetPosition(x, y, z); };
		explicit Transform(GameObject* ownerPtr, const float x, const float y) : Component(ownerPtr) { SetPosition(x, y); };
		explicit Transform(GameObject* ownerPtr, int x, int y, int z) : Component(ownerPtr) { SetPosition(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)); };
		explicit Transform(GameObject* ownerPtr, int x, int y) : Component(ownerPtr) { SetPosition(static_cast<float>(x), static_cast<float>(y)); };
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
		void SetPosition(const glm::vec3 pos) { m_Position = pos; };
		void SetPosition(const glm::vec2 pos) { m_Position.x = pos.x; m_Position.y = pos.y; };
		void SetMovement(const Movement& movement) { m_LastMovement = m_CurrentMovement; m_CurrentMovement = movement; };
		void SetLookingDirection(const Movement& movement) { m_LookingDirection = movement; };
		Movement GetMovement() const { return m_CurrentMovement; };
		Movement GetLastMovement() const { return m_LastMovement; };
		Movement GetLookingDirection() const { return m_LookingDirection; };
		glm::vec3 GetMovementVector(const float value) const;
		glm::vec2 Get2DMovementVector(const float value) const;
		glm::vec2 Get2DMovementVector(const Movement& movement, const float value) const;

		mutable glm::vec2 m_target = { -2, -2 };
		mutable glm::vec2 blinky = { -2, -2 };
		mutable glm::vec2 firstTarget = { -2, -2 };

	private:
		glm::vec3 m_Position;
		Movement m_CurrentMovement;
		Movement m_LastMovement;
		Movement m_LookingDirection;
	};
}
