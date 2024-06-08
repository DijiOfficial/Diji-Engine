#pragma once
#include "Subject.h"
#include "Component.h"

namespace diji
{
	class Collider;
}

namespace pacman
{
	class GhostState;
	class AI;

	class GhostCollision final : public diji::Component, public diji::Subject
	{
	public:
		GhostCollision(diji::GameObject* ownerPtr, const diji::GameObject* player);
		~GhostCollision() noexcept override = default;

		void Init() override;
		void Update() override;
		void FixedUpdate() override {};
		void AddPLayer2Collider(diji::Collider* player2) { m_Player2ColliderPtr = player2; };
		GhostState* GetCurrentState();
		int GetGhostsEaten() const;

		const diji::GameObject* GetParent() const { return GetOwner(); }
	private:
		diji::Collider* m_PlayerColliderPtr;
		diji::Collider* m_Player2ColliderPtr = nullptr;
		diji::Collider* m_OwnerColliderPtr;
		AI* m_PlayerAIPtr;
	};
}

