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

	for (const auto& posVec : m_PelletsVec)
	{
		for (const auto& pos : posVec)
		{
			AddPickUp("bruh.png", 4, 4, pos, 10, pelletCounter);
		}
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
	pickUp->AddComponents<ScoreCounter>(0);
	pickUp->AddComponents<PickUp>(m_PlayerPtr, pelletCouter, value);
	pickUp->GetComponent<Render>()->EnableHitbox();

	pickUp->GetComponent<PickUp>()->AddObserver(static_cast<MessageTypes>(MessageTypesDerived::PICKUP_COLLISION), m_PlayerPtr->GetComponent<AI>());
	pickUp->GetComponent<PickUp>()->AddObserver(static_cast<MessageTypes>(MessageTypesDerived::PICKUP_COLLISION), pelletCouter->GetComponent<PelletObserver>());

}
