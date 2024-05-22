#include "Observers.h"

#include "HealthCounter.h"
#include "ScoreCounter.h"
#include "PickUp.h"
#include <format>
void pacman::HealthObserver::OnNotify(diji::MessageTypes message, diji::Subject* subject)
{
	auto msg = static_cast<MessageTypesDerived>(message);
	if (msg == MessageTypesDerived::HEALTH_CHANGE)
	{
		HealthCounter* healthCounter = dynamic_cast<HealthCounter*>(subject);
		if (not healthCounter)
			return; // throw error instead

		const int lives = healthCounter->GetHealth();
		std::string text = "lives";
		if (lives == 1)
		{
			text = "life";
		}
		SetText(std::format("# {}: {}", text, lives));
	}
}

void pacman::ScoreObserver::OnNotify(diji::MessageTypes message, diji::Subject* subject)
{
	auto msg = static_cast<MessageTypesDerived>(message);
	if (msg == MessageTypesDerived::SCORE_CHANGE)
	{
		ScoreCounter* scoreCounter = dynamic_cast<ScoreCounter*>(subject);
		if (not scoreCounter)
			return;

		const int score = scoreCounter->GetScore();
		SetText(std::format("Score: {}", score));
	}
}

void pacman::PelletObserver::OnNotify(diji::MessageTypes message, diji::Subject* subject)
{
	auto msg = static_cast<MessageTypesDerived>(message);
	if (msg == MessageTypesDerived::PICKUP_COLLISION)
	{
		PickUp* pickup = dynamic_cast<PickUp*>(subject);
		if (not pickup)
			return;

		++m_PelletCount;
	}
}
