#include "PickUpManager.h"
#include "Components.h"
#include "GameObject.h"
#include "SVGParser.h"
//diji::PickUpManager::PickUpManager(GameObject* player)
//{
//	m_ScenePtr = SceneManager::GetInstance().CreateScene("PickUpManager");
//	m_PlayerPtr = player;
//
//	Initialize();
//}

void diji::PickUpManager::Initialize(GameObject* player)
{
	SVGParser::GetVerticesFromSvgFile("Pellets.svg", m_PelletsVec, 78);
		
	m_ScenePtr = SceneManager::GetInstance().CreateScene("PickUpManager");
	m_PlayerPtr = player;

	for (const auto& posVec : m_PelletsVec)
	{
		for (const auto& pos : posVec)
		{
			AddPickUp("bruh.png", 4, 4, pos, 10);
		}
	}
}

void diji::PickUpManager::AddPickUp(const std::string& file, const int width, const int height, const glm::vec2& pos, const int value)
{
	auto pickUp = m_ScenePtr->CreateGameObject();
	pickUp->AddComponents<Texture>(file, width, height);
	pickUp->AddComponents<Transform>(pos.x, pos.y);
	pickUp->AddComponents<Render>(2);
	pickUp->AddComponents<Collider>(width, height);
	pickUp->AddComponents<PickUp>(value);

	pickUp->GetComponent<PickUp>()->AddObserver(MessageTypes::PICKUP_COLLISION, m_PlayerPtr->GetComponent<AI>());

}
