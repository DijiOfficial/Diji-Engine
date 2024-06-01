#pragma once
#include "Component.h"
#include <vector>
#include "IObserver.h"

namespace diji 
{
	class GameObject;
}

namespace pacman 
{
	class GhostsTimers final : public diji::Component, public diji::IObserver
	{
	public:
		GhostsTimers(diji::GameObject* ownerPtr) : Component(ownerPtr) {};
		~GhostsTimers() noexcept = default;

		GhostsTimers(const GhostsTimers& other) = delete;
		GhostsTimers(GhostsTimers&& other) = delete;
		GhostsTimers& operator=(const GhostsTimers& other) = delete;
		GhostsTimers& operator=(GhostsTimers&& other) = delete;

		void Init() override {};
		void Update();
		void FixedUpdate() override {};
		void OnNotify(diji::MessageTypes message, diji::Subject* subject) override;

		bool IsInChaseState() const { return m_IsInChaseState; }

		bool IsPaused() const { return m_IsPaused or m_IsInIntro; };
		void Pause() { m_PauseTimer = 0.f; m_IsPaused = true; };
	private:
		bool m_IsInIntro = true;
		bool m_IsInChaseState = false;
		bool m_IsPaused = false;
		int m_CurrentCycle = 0;
		float m_TotalElapsedTime = 0.0f;
		float m_PauseTimer = -12.0f;
		std::vector<float> m_ChaseScatterDuration = { 7.0f, 20.0f, 7.0f, 20.0f, 5.0f, 20.0f, 5.0f };
	};
}

