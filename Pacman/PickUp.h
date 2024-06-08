#pragma once
#include "Subject.h"
#include "Component.h"
#include "IObserver.h"

namespace diji 
{
	enum class SoundId;
	class GameObject;
	class Collider;
	class Render;
}

namespace pacman
{
	class PelletObserver;
	class PickUp final : public diji::Component, public diji::Subject, public diji::IObserver
	{
	public:
		PickUp(diji::GameObject* ownerPtr, const diji::GameObject* player, const diji::GameObject* player2, const diji::GameObject* pelletCounter, const int value);
		PickUp(diji::GameObject* ownerPtr, const diji::GameObject* player, const diji::GameObject* player2, const int value);
		~PickUp() noexcept override = default;

		PickUp(const PickUp& other) = delete;
		PickUp(PickUp&& other) = delete;
		PickUp& operator=(const PickUp& other) = delete;
		PickUp& operator=(PickUp&& other) = delete;

		void Init() override;
		void Update() override;
		void FixedUpdate() override {};
		void OnNotify(diji::MessageTypes message, diji::Subject* subject) override;
		int GetValue() const { return m_Value; };
		void HandleCollision();

	private:
		int m_PowerUpInvisibleFrames = 0;
		const int m_Value;
		diji::SoundId m_SoundId;
		PelletObserver* m_PelletCounter;
		diji::Collider* m_PlayerColliderPtr;
		diji::Collider* m_Player2ColliderPtr;
		diji::Collider* m_OwnerColliderPtr;
		diji::Render* m_RenderCompPtr;
		bool m_IsPowerUp = false;
		bool m_IsDisabled = false;
	};
}

