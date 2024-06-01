#include "PickUpLoader.h"
#include "Components.h"
#include "GameObject.h"
#include "SVGParser.h"

#include "Observers.h"
#include "AI.h"
#include "PickUp.h"
#include "GhostAI.h"
#include "SceneManager.h"
#include "GameState.h"
#include "PelletCounter.h"
pacman::PickUpLoader::PickUpLoader(const diji::GameObject* player, const std::vector<diji::GameObject*>& gameObjects, const diji::GameObject* pelletCounter)
{
	diji::SVGParser::GetVerticesFromSvgFile("Pellets.svg", m_PelletsVec, 78);

	//todo: Get Level scene
	//m_ScenePtr = diji::SceneManager::GetInstance().CreateScene("PickUpLoader");
	m_ScenePtr = diji::SceneManager::GetInstance().GetScene(static_cast<int>(pacman::GameState::LEVEL));
	m_PlayerPtr = player;

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
				AddPowerUp(gameObjects, "PowerPellet.png", 8, 8, pos, 50, pelletCounter);
			}
		++idx;
	}
}

void pacman::PickUpLoader::AddPickUp(const std::string& file, const int width, const int height, const glm::vec2& pos, const int value, const diji::GameObject* pelletCouter)
{
	auto pickUp = m_ScenePtr->CreateGameObject();
	pickUp->AddComponents<diji::Texture>(file, width, height);
	pickUp->AddComponents<diji::Transform>(pos.x, pos.y);
	pickUp->AddComponents<diji::Render>(2);
	//pickUp->AddComponents<Collider>(width, height);
	pickUp->AddComponents<diji::Collider>(1, 1, glm::vec2{ 2, 2 });
	//pickUp->AddComponents<ScoreCounter>(0);
	pickUp->AddComponents<PickUp>(m_PlayerPtr, pelletCouter, value);

	pickUp->GetComponent<PickUp>()->AddObserver(static_cast<diji::MessageTypes>(MessageTypesDerived::PICKUP_COLLISION), m_PlayerPtr->GetComponent<AI>());
	pickUp->GetComponent<PickUp>()->AddObserver(static_cast<diji::MessageTypes>(MessageTypesDerived::PICKUP_COLLISION), pelletCouter->GetComponent<PelletObserver>());
	pelletCouter->GetComponent<PelletCounter>()->AddObserver(static_cast<diji::MessageTypes>(MessageTypesDerived::LEVEL_END), pickUp->GetComponent<PickUp>());
}

void pacman::PickUpLoader::AddPowerUp(const std::vector<diji::GameObject*>& gameObjects, const std::string& file, const int width, const int height, const glm::vec2& pos, const int value, const diji::GameObject* pelletCouter)
{
	auto powerUp = m_ScenePtr->CreateGameObject();
	powerUp->AddComponents<diji::Texture>(file, width, height);
	powerUp->AddComponents<diji::Transform>(pos.x, pos.y);
	powerUp->AddComponents<diji::Render>(2);
	powerUp->AddComponents<diji::Collider>(width, height);
	powerUp->AddComponents<PickUp>(m_PlayerPtr, value);

	powerUp->GetComponent<PickUp>()->AddObserver(static_cast<diji::MessageTypes>(MessageTypesDerived::POWERUP_COLLISION), m_PlayerPtr->GetComponent<AI>());
	for (const auto& object : gameObjects)
	{
		powerUp->GetComponent<PickUp>()->AddObserver(static_cast<diji::MessageTypes>(MessageTypesDerived::POWERUP_COLLISION), object->GetComponent<GhostAI>());
	}

	pelletCouter->GetComponent<PelletCounter>()->AddObserver(static_cast<diji::MessageTypes>(MessageTypesDerived::LEVEL_END), powerUp->GetComponent<PickUp>());
}