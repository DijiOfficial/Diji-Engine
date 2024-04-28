#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Scene.h"
#include "Singleton.h"

namespace diji
{
	class PickUp;
	class GameObject;

	//it's a singleton for now, Im not sure how to implement it otherwise
	class PickUpManager final : public Singleton<PickUpManager>
	{
	public:
		//PickUpManager(GameObject* player);
		//~PickUpManager() = default;


		PickUpManager(const PickUpManager& other) = delete;
		PickUpManager(PickUpManager&& other) = delete;
		PickUpManager& operator=(const PickUpManager& other) = delete;
		PickUpManager& operator=(PickUpManager&& other) = delete;

		void Initialize(GameObject* player);

		void AddPickUp(const std::string& file, const int width, const int height, const glm::vec2& pos, const int value);
		
	private:
		friend class Singleton<PickUpManager>;
		PickUpManager() = default;
		std::vector<std::vector<glm::vec2>> m_PelletsVec;

		Scene* m_ScenePtr = nullptr;
		GameObject* m_PlayerPtr = nullptr;
	};
}
