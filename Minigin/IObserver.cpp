#include "IObserver.h"

#include "HealthCounter.h"
#include "ScoreCounter.h"

void diji::HealthObserver::OnNotify(MessageTypes message, Subject* subject)
{ 
	if (message == MessageTypes::HEALTH_CHANGE)
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

void diji::ScoreObserver::OnNotify(MessageTypes message, Subject* subject)
{
	if (message == MessageTypes::SCORE_CHANGE)
	{
		ScoreCounter* scoreCounter = dynamic_cast<ScoreCounter*>(subject);
		if (not scoreCounter)
			return;

		const int score = scoreCounter->GetScore();
		SetText(std::format("Score: {}", score));
	}
}
