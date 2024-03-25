#include "IObserver.h"
#include "Text.h"
#include "HealthCounter.h"
#include "ScoreCounter.h"
#include <cassert>

#include <iostream>

diji::HealthObserver::HealthObserver(GameObject* gameObj, GameObject* subject)
	: IObserver(subject)
{
	m_TextCompPtr = { gameObj->GetComponent<Text>() };

	assert(m_TextCompPtr and "Text Component needs to be initialized before HealthObserver");

}
void diji::HealthObserver::OnNotify(const GameObject* entity)
{
	if (entity == m_Subject) return;

	const int lives = entity->GetComponent<HealthCounter>()->GetHealth();
	
	std::string text = "lives";
	if (lives == 1)
	{
		text = "life";
	}

	m_TextCompPtr->SetText(std::format("# {}: {}", text, lives));

}

diji::ScoreObserver::ScoreObserver(GameObject* observer, GameObject* subject)
	: IObserver(subject)
{
	m_TextCompPtr = { observer->GetComponent<Text>() };

	assert(m_TextCompPtr and "Text Component needs to be initialized before ScoreObserver");
}

void diji::ScoreObserver::OnNotify(const GameObject* entity)
{
	if (entity == m_Subject) return;
	const int score = entity->GetComponent<ScoreCounter>()->GetScore();

	m_TextCompPtr->SetText(std::format("Score: {}", score));
}