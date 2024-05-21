#pragma once
#include "Command.h"
#include "IObserver.h"

namespace diji {
	class Texture;
	class Transform;
	class Collider;
	struct Rectf;
	
	class GhostAI;

	class AI final : public Component, public IObserver
	{
	public:
		AI(GameObject* ownerPtr);
		~AI() override = default;

		AI(const AI& other) = delete;
		AI(AI&& other) = delete;
		AI& operator=(const AI& other) = delete;
		AI& operator=(AI&& other) = delete;

		void Update() override;
		void FixedUpdate() override;
		void OnNotify(MessageTypes message, [[maybe_unused]] Subject* subject) override;
		bool GetIsPoweredUp() const { return m_IsPoweredUp; };

		static constexpr float TOTAL_WIDTH = 452;
	private:
		Texture* m_TextureCompPtr;
		Transform* m_TransformCompPtr;
		Collider* m_ColliderCompPtr;
		Movement m_PreviousMovement = Movement::Right;
		Movement m_SavedMovement = Movement::Right;

		const glm::vec2 m_Speed = { 160.f, 160.f };
		//std::unique_ptr<PlayerState> m_CurrentStateUPtr;
		bool m_IsPoweredUp = false;
		float PowerUpTimer = 0.f;

		const Rectf CalculateNewPosition(Movement movement);
	};
}

