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

	class GhostCollision final : public diji::Component, public diji::Subject
	{
	public:
		GhostCollision(diji::GameObject* ownerPtr, const diji::GameObject* player);
		~GhostCollision() noexcept override = default;

		void Init() override;
		void Update() override;
		void FixedUpdate() override {};

		GhostState* GetCurrentState();

	private:
		diji::Collider* m_PlayerColliderPtr;
		diji::Collider* m_OwnerColliderPtr;
	};
}

