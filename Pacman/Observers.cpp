#include "Observers.h"
#include "GameObject.h"
#include "HealthCounter.h"
#include "ScoreCounter.h"
#include "PickUp.h"
#include "Texture.h"

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

pacman::PacmanHealthObserver::PacmanHealthObserver(diji::GameObject* ownerPtr)
	: Component(ownerPtr)
{
}

void pacman::PacmanHealthObserver::Init()
{
	m_TextureCompPtr = GetOwner()->GetComponent<diji::Texture>();
}

#include <iostream>
void pacman::PacmanHealthObserver::OnNotify(diji::MessageTypes message, diji::Subject* subject)
{
	std::cout << "decreasece health::OnNotify" << std::endl;
	auto msg = static_cast<MessageTypesDerived>(message);
	if (msg == MessageTypesDerived::HEALTH_CHANGE)
	{
		HealthCounter* healthCounter = dynamic_cast<HealthCounter*>(subject);
		if (not healthCounter)
			return;

		const int health = healthCounter->GetHealth() >= 0 ? healthCounter->GetHealth() : 0;
		m_TextureCompPtr->SetWidth(M_LIFE_TEXTURE_WIDTH * health);
	}
}
