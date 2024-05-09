#pragma once
#include "IObserver.h"
#include "Text.h"

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
}

