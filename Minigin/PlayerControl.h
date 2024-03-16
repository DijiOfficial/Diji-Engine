#pragma once
#include "GameObject.h"

namespace diji 
{
	class Transform;

	class PlayerControl final : public Component
	{
	public:
		PlayerControl(GameObject* ownerPtr);
		~PlayerControl() override = default;

		PlayerControl(const PlayerControl& other) = delete;
		PlayerControl(PlayerControl&& other) = delete;
		PlayerControl& operator=(const PlayerControl& other) = delete;
		PlayerControl& operator=(PlayerControl&& other) = delete;

		void Update() override;
	
	private:
		Transform* m_TransformComponentPtr;
		glm::vec3 m_Speed = { 100.f, 100.f, 100.f };
	};
}

