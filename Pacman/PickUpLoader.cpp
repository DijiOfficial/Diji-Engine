#include "PickUpLoader.h"
#include "Components.h"
#include "GameObject.h"
#include "SVGParser.h"

#include "Observers.h"
#include "AI.h"
#include "PickUp.h"

//void diji::PickUpLoader::Initialize(GameObject* player)
//{
//	SVGParser::GetVerticesFromSvgFile("Pellets.svg", m_PelletsVec, 78);
//
//	m_ScenePtr = SceneManager::GetInstance().CreateScene("PickUpLoader");
//	m_PlayerPtr = player;
//
//	for (const auto& posVec : m_PelletsVec)
//	{
//		for (const auto& pos : posVec)
//		{
//			AddPickUp("bruh.png", 4, 4, pos, 10);
//		}
//	}
//}

diji::PickUpLoader::PickUpLoader(GameObject* player)
{
	SVGParser::GetVerticesFromSvgFile("Pellets.svg", m_PelletsVec, 78);

	m_ScenePtr = SceneManager::GetInstance().CreateScene("PickUpLoader");
	m_PlayerPtr = player;

	for (const auto& posVec : m_PelletsVec)
	{
		for (const auto& pos : posVec)
		{
			AddPickUp("bruh.png", 4, 4, pos, 10);
		}
	}
}

void diji::PickUpLoader::AddPickUp(const std::string& file, const int width, const int height, const glm::vec2& pos, const int value)
{
	auto pickUp = m_ScenePtr->CreateGameObject();
	pickUp->AddComponents<Texture>(file, width, height);
	pickUp->AddComponents<Transform>(pos.x, pos.y);
	pickUp->AddComponents<Render>(2);
	pickUp->AddComponents<Collider>(width, height);
	pickUp->AddComponents<PickUp>(m_PlayerPtr, value);

	pickUp->GetComponent<PickUp>()->AddObserver(static_cast<MessageTypes>(MessageTypesDerived::PICKUP_COLLISION), m_PlayerPtr->GetComponent<AI>());

}
