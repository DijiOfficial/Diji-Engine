#pragma once
#include "IObserver.h"
#include "Text.h"
#include "Component.h"

namespace diji 
{
	enum class MessageTypesDerived
	{
		HEALTH_CHANGE,
		SCORE_CHANGE,
		LEVEL_COLLISION,
		ENEMY_COLLISION,
		PICKUP_COLLISION,
	};

	class HealthObserver final : public Text, public IObserver
	{
	public:
		using Text::Text;
		~HealthObserver() override = default;

		void OnNotify(MessageTypes message, Subject* subject) override;
	};

	class ScoreObserver final : public Text, public IObserver
	{
	public:
		using Text::Text;
		~ScoreObserver() override = default;

		void OnNotify(MessageTypes message, Subject* subject) override;
	};

	class PelletObserver final : public Component, public IObserver
	{
	public:
		PelletObserver(GameObject* ownerPtr) : Component(ownerPtr) {};
		~PelletObserver() override = default;

		void OnNotify(MessageTypes message, Subject* subject) override;
		void Update() override {};
		void FixedUpdate() override {};
		int GetPelletCount() const { return m_PelletCount; }
	
	private:
		int m_PelletCount = 0;
	};
}

