#pragma once
#include "IObserver.h"
#include "Text.h"
#include "Component.h"

namespace pacman 
{
	enum class MessageTypesDerived
	{
		HEALTH_CHANGE,
		SCORE_CHANGE,
		LEVEL_COLLISION,
		ENEMY_COLLISION,
		PICKUP_COLLISION,
		POWERUP_COLLISION,
	};

	class HealthObserver final : public diji::Text, public diji::IObserver
	{
	public:
		using Text::Text;
		~HealthObserver() override = default;

		void OnNotify(diji::MessageTypes message, diji::Subject* subject) override;
	};

	class ScoreObserver final : public diji::Text, public diji::IObserver
	{
	public:
		using Text::Text;
		~ScoreObserver() override = default;

		void OnNotify(diji::MessageTypes message, diji::Subject* subject) override;
	};

	class PelletObserver final : public diji::Component, public diji::IObserver
	{
	public:
		PelletObserver(diji::GameObject* ownerPtr) : Component(ownerPtr) {};
		~PelletObserver() override = default;

		void OnNotify(diji::MessageTypes message, diji::Subject* subject) override;
		void Update() override {};
		void FixedUpdate() override {};
		int GetPelletCount() const { return m_PelletCount; }
	
	private:
		int m_PelletCount = 0;
	};
}

