#pragma once
#include "Component.h"

namespace pacman
{
	class Timer final : public diji::Component
	{
	public:
		Timer(diji::GameObject* ownerPtr) : Component(ownerPtr) {};
		~Timer() noexcept override = default;

		void Init() override {};
		void Update() override;
		void FixedUpdate() override {};

		float GetTime() const { return m_Timer; }
		void DisableTimer() { m_IsActive = false; }

	private:
		float m_Timer = 0.0f;
		bool m_IsActive = true;
	};
}