#pragma once
#include "Subject.h"
#include "Component.h"

namespace diji
{
	enum class SoundId;
	class GameObject;
	class Collider;
	class PelletObserver;
	class Render;
	class PickUp final : public Component, public Subject
	{
	public:
		PickUp(GameObject* ownerPtr, const GameObject* player, const GameObject* pelletCounter, const int value);
		PickUp(GameObject* ownerPtr, const GameObject* player, const int value);
		~PickUp() override = default;

		PickUp(const PickUp& other) = delete;
		PickUp(PickUp&& other) = delete;
		PickUp& operator=(const PickUp& other) = delete;
		PickUp& operator=(PickUp&& other) = delete;

		void Update() override;
		void FixedUpdate() override {};
		int GetValue() const { return m_Value; };
		void HandleCollision();

	private:
		const int m_Value;
		SoundId m_SoundId;
		Collider* m_PlayerColliderPtr;
		Collider* m_OwnerColliderPtr;
		PelletObserver* m_PelletCounter;
		Render* m_RenderCompPtr;
		bool m_IsPowerUp = false;
		bool m_IsDisabled = false;
		int m_PowerUpInvisibleFrames = 0;
	};
}

