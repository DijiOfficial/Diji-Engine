#pragma once
#include "Subject.h"
#include "Component.h"

namespace pacman
{
	class LevelIntro final : public diji::Component, public diji::Subject
	{
	public:
		LevelIntro(diji::GameObject* ownerPtr) : Component(ownerPtr) {};
		~LevelIntro() noexcept override = default;

		void Init() override {};
		void Update() override;
		void FixedUpdate() override {};

	private:
		bool m_MusicStarted = false;
		bool m_SpawnNotify = true;
		bool m_StartLevel = false;
		float m_Timer = 0.f;
	};
}

