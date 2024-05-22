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
		~AI() override = default;

		AI(const AI& other) = delete;
		AI(AI&& other) = delete;
		AI& operator=(const AI& other) = delete;
		AI& operator=(AI&& other) = delete;

		void Init() override;
		void Update() override;
		void FixedUpdate() override;
		void OnNotify(diji::MessageTypes message, [[maybe_unused]] diji::Subject* subject) override;
		bool GetIsPoweredUp() const { return m_IsPoweredUp; };

		static constexpr float TOTAL_WIDTH = 452;
	private:
		diji::Texture* m_TextureCompPtr;
		diji::Transform* m_TransformCompPtr;
		diji::Collider* m_ColliderCompPtr;
		diji::Movement m_PreviousMovement = diji::Movement::Right;
		diji::Movement m_SavedMovement = diji::Movement::Right;

		const glm::vec2 m_Speed = { 160.f, 160.f };
		//std::unique_ptr<PlayerState> m_CurrentStateUPtr;
		bool m_IsPoweredUp = false;
		float PowerUpTimer = 0.f;

		const diji::Rectf CalculateNewPosition(diji::Movement movement);
	};
}

