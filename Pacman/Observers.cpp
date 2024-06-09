#include "Observers.h"
#include "GameObject.h"
#include "HealthCounter.h"
#include "ScoreCounter.h"
#include "PickUp.h"
#include "Texture.h"
#include "Render.h"
#include "ScoreBoard.h"
#include "Transform.h"

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
		const int numDigits = score > 0 ? static_cast<int>(std::log10(score)) + 1 : 1;
		const int numSpaces = std::max(0, 7 - numDigits);
		const std::string formattedScore = std::format("{:>{}}", score, numSpaces + numDigits);

		SetText(formattedScore);
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

void pacman::PacmanHealthObserver::OnNotify(diji::MessageTypes message, diji::Subject* subject)
{
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

pacman::IntroTextObserver::IntroTextObserver(diji::GameObject* ownerPtr, MessageTypesDerived message)
	: Component(ownerPtr)
	, m_Message{ message }
{
}

void pacman::IntroTextObserver::Init()
{
	m_RenderCompPtr = GetOwner()->GetComponent<diji::Render>();
}

void pacman::IntroTextObserver::OnNotify(diji::MessageTypes message, diji::Subject* subject)
{
	(void)subject;
	auto msg = static_cast<MessageTypesDerived>(message);
	if (static_cast<int>(msg) == static_cast<int>(m_Message))
		m_RenderCompPtr->DisableRender();
}

pacman::HighScoreObserver::HighScoreObserver(diji::GameObject* ownerPtr, std::string text, diji::Font* font, const SDL_Color& color, bool isCentered)
	: Text(ownerPtr, text, font, color, isCentered)
{
	m_CurrentHighScore = ScoreBoard::GetInstance().GetHighestScore();
	const int numDigits = m_CurrentHighScore > 0 ? static_cast<int>(std::log10(m_CurrentHighScore)) + 1 : 1;
	const int numSpaces = std::max(0, 7 - numDigits);
	const std::string formattedScore = std::format("{:>{}}", m_CurrentHighScore, numSpaces + numDigits);
	
	SetText(formattedScore);
}

void pacman::HighScoreObserver::OnNotify(diji::MessageTypes message, diji::Subject* subject)
{
	auto msg = static_cast<MessageTypesDerived>(message);
	if (msg == MessageTypesDerived::SCORE_CHANGE)
	{
		ScoreCounter* scoreCounter = dynamic_cast<ScoreCounter*>(subject);
		if (not scoreCounter)
			return;
		
		const int score = scoreCounter->GetScore();
		if (score <= m_CurrentHighScore)
			return;

		const int numDigits = score > 0 ? static_cast<int>(std::log10(score)) + 1 : 1;
		const int numSpaces = std::max(0, 7 - numDigits);
		const std::string formattedScore = std::format("{:>{}}", score, numSpaces + numDigits);

		SetText(formattedScore);
	}
}

void pacman::HighScoreObserver::DisplayPossibleScore(int score)
{
	if (score <= m_CurrentHighScore)
		return;

	const int numDigits = score > 0 ? static_cast<int>(std::log10(score)) + 1 : 1;
	const int numSpaces = std::max(0, 7 - numDigits);
	const std::string formattedScore = std::format("{:>{}}", score, numSpaces + numDigits);

	SetText(formattedScore);
}

void pacman::LevelObserver::OnNotify(diji::MessageTypes message, diji::Subject* subject)
{
	(void)subject;
	auto msg = static_cast<MessageTypesDerived>(message);
	if (msg == MessageTypesDerived::LEVEL_END)
	{
		++m_LevelCount;
		if (m_LevelCount < 8)
		{
			GetOwner()->GetComponent<diji::Texture>()->SetWidth(16 * m_LevelCount);
			
			const auto& transform = GetOwner()->GetComponent<diji::Transform>();
			transform->SetPosition(transform->GetPosition().x - 32, transform->GetPosition().y);
		}
		if (m_LevelCount < 20)
		{
			const auto& texture = GetOwner()->GetComponent<diji::Texture>();
			texture->SetCurrentFrame(texture->GetFrame() - 1);
		}
	}
}
