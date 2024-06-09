#pragma once
#include "Component.h"
#include "IObserver.h"
#include "Command.h"
#include <glm/glm.hpp>

namespace diji
{
	class Texture;
	class Transform;
	class Collider;
	struct Rectf;
}

namespace pacman {
	
	class GhostAI;

	class AI final : public diji::Component, public diji::IObserver
	{
	public:
		AI(diji::GameObject* ownerPtr);
		~AI() noexcept override = default;

		AI(const AI& other) = delete;
		AI(AI&& other) = delete;
		AI& operator=(const AI& other) = delete;
		AI& operator=(AI&& other) = delete;

		void Init() override;
		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;
		void OnNotify(diji::MessageTypes message, [[maybe_unused]] diji::Subject* subject) override;

		void SetInitialMovementLeft() { m_PreviousMovement = diji::Movement::Left; m_SavedMovement = diji::Movement::Left; };
		int GetGhostsEaten() const { return m_GhostsEaten; }
		static constexpr float TOTAL_WIDTH = 452;
	private:
		diji::Texture* m_TextureCompPtr;
		diji::Transform* m_TransformCompPtr;
		diji::Collider* m_ColliderCompPtr;
		diji::Movement m_PreviousMovement = diji::Movement::Right;
		diji::Movement m_SavedMovement = diji::Movement::Right;

		const glm::vec2 m_Speed = { 160.f, 160.f };
		const diji::Rectf CalculateNewPosition(diji::Movement movement);
		bool m_PauseAI = true;
		//bool m_IsPoweredUp = false;
		//float m_PowerUpTimer = 0.f;
		float m_PauseTime = -10.f;
		int m_GhostsEaten = 0;
		bool m_IsDying = false;
		bool m_playeDeath = true;

		bool m_TeleportedThisFrame = false;

		bool IsGhostFrightened(diji::Subject* subject) const;
		void Reset();
		bool CheckIfDirectionIsValid(const diji::Movement& movement);
	};
}

