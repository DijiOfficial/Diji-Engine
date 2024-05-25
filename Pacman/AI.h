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
		void OnNotify(diji::MessageTypes message, [[maybe_unused]] diji::Subject* subject) override;

		static constexpr float TOTAL_WIDTH = 452;
	private:
		diji::Texture* m_TextureCompPtr;
		diji::Transform* m_TransformCompPtr;
		diji::Collider* m_ColliderCompPtr;
		diji::Movement m_PreviousMovement = diji::Movement::Right;
		diji::Movement m_SavedMovement = diji::Movement::Right;

		const glm::vec2 m_Speed = { 160.f, 160.f };
		const diji::Rectf CalculateNewPosition(diji::Movement movement);
		bool m_PauseAI = false;
		float m_PauseTime = 0.f;
	};
}

