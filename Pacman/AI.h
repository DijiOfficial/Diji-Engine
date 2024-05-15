#pragma once
#include "Command.h"
#include "IObserver.h"

namespace diji {
	class Texture;
	class Transform;
	class Collider;
	struct Rectf;
	//class PlayerState
	//{
	//public:
	//	PlayerState() = default;
	//	virtual ~PlayerState() = default;

	//	PlayerState(const PlayerState& other) = delete;
	//	PlayerState(PlayerState&& other) = delete;
	//	PlayerState& operator=(const PlayerState& other) = delete;
	//	PlayerState& operator=(PlayerState&& other) = delete;

	//	virtual std::unique_ptr<PlayerState> Execute(Transform* transform, Collider* collider) = 0;

	//	virtual void OnEnter() = 0;
	//	virtual void OnExit() = 0;
	//};

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

	private:
		Texture* m_TextureCompPtr;
		Transform* m_TransformCompPtr;
		Collider* m_ColliderCompPtr;
		Movement m_PreviousMovement = Movement::Right;
		Movement m_SavedMovement = Movement::Right;

		const float TOTAL_WIDTH = 452;
		const glm::vec2 m_Speed = { 160.f, 160.f };
		//std::unique_ptr<PlayerState> m_CurrentStateUPtr;

		const Rectf CalculateNewPosition(Movement movement);
		void SmoothOutCollision(Rectf& shape, const Movement& movement);
	};
}

