#pragma once
#include "Text.h"

namespace diji 
{
	enum class MessageTypes
	{
		HEALTH_CHANGE,
		SCORE_CHANGE,
		LEVEL_COLLISION,
		ENEMY_COLLISION,
		PICKUP_COLLISION
	};
	class Subject;

	class IObserver
	{
	public:
		IObserver() = default;
		virtual ~IObserver() = default;

		IObserver(const IObserver& other) = delete;
		IObserver(IObserver&& other) = delete;
		IObserver& operator=(const IObserver& other) = delete;
		IObserver& operator=(IObserver&& other) = delete;

		virtual void OnNotify(MessageTypes message, Subject* subject) = 0;
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