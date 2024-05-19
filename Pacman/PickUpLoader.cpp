#include "PickUpLoader.h"
#include "Components.h"
#include "GameObject.h"
#include "SVGParser.h"

#include "Observers.h"
#include "AI.h"
#include "PickUp.h"
#include "ScoreCounter.h"

diji::PickUpLoader::PickUpLoader(GameObject* player)
{
	SVGParser::GetVerticesFromSvgFile("Pellets.svg", m_PelletsVec, 78);

	m_ScenePtr = SceneManager::GetInstance().CreateScene("PickUpLoader");
	m_PlayerPtr = player;

	auto pelletCounter = m_ScenePtr->CreateGameObject();
	pelletCounter->AddComponents<PelletObserver>();

	int idx = 0;
	for (const auto& posVec : m_PelletsVec)
	{
		if (idx == 0)
			for (const auto& pos : posVec)
			{
				AddPickUp("bruh.png", 4, 4, pos, 10, pelletCounter);
			}
		else
			for (const auto& pos : posVec)
			{
				AddPowerUp("PowerPellet.png", 8, 8, pos, 50);
			}
		++idx;
	}
}

void diji::PickUpLoader::AddPickUp(const std::string& file, const int width, const int height, const glm::vec2& pos, const int value, const GameObject* pelletCouter)
{
	auto pickUp = m_ScenePtr->CreateGameObject();
	pickUp->AddComponents<Texture>(file, width, height);
	pickUp->AddComponents<Transform>(pos.x, pos.y);
	pickUp->AddComponents<Render>(2);
	//pickUp->AddComponents<Collider>(width, height);
	pickUp->AddComponents<Collider>(1, 1, glm::vec2{ 2, 2 });
	//pickUp->AddComponents<ScoreCounter>(0);
	pickUp->AddComponents<PickUp>(m_PlayerPtr, pelletCouter, value);

	pickUp->GetComponent<PickUp>()->AddObserver(static_cast<MessageTypes>(MessageTypesDerived::PICKUP_COLLISION), m_PlayerPtr->GetComponent<AI>());
	pickUp->GetComponent<PickUp>()->AddObserver(static_cast<MessageTypes>(MessageTypesDerived::PICKUP_COLLISION), pelletCouter->GetComponent<PelletObserver>());

}

void diji::PickUpLoader::AddPowerUp(const std::string& file, const int width, const int height, const glm::vec2& pos, const int value)
{
	auto powerUp = m_ScenePtr->CreateGameObject();
	powerUp->AddComponents<Texture>(file, width, height);
	powerUp->AddComponents<Transform>(pos.x, pos.y);
	powerUp->AddComponents<Render>(2);
	powerUp->AddComponents<Collider>(width, height);
	powerUp->AddComponents<PickUp>(m_PlayerPtr, value);

	powerUp->GetComponent<PickUp>()->AddObserver(static_cast<MessageTypes>(MessageTypesDerived::POWERUP_COLLISION), m_PlayerPtr->GetComponent<AI>());
}