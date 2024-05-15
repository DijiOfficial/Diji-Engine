#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Scene.h"

namespace diji
{
	class PickUp;
	class GameObject;

		//class PickUpLoader final : public Singleton<PickUpLoader>
	class PickUpLoader final
	{
	public:
		PickUpLoader(GameObject* player);
		~PickUpLoader() = default;


		PickUpLoader(const PickUpLoader& other) = delete;
		PickUpLoader(PickUpLoader&& other) = delete;
		PickUpLoader& operator=(const PickUpLoader& other) = delete;
		PickUpLoader& operator=(PickUpLoader&& other) = delete;

		//void Initialize(GameObject* player);

		void AddPickUp(const std::string& file, const int width, const int height, const glm::vec2& pos, const int value, const GameObject* pelletCounter);
	private:
		//friend class Singleton<PickUpLoader>;
		//PickUpLoader() = default;
		std::vector<std::vector<glm::vec2>> m_PelletsVec;

		Scene* m_ScenePtr = nullptr;
		GameObject* m_PlayerPtr = nullptr;
	};
}
