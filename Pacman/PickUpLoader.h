#pragma once
#include "Scene.h"
#include <glm/glm.hpp>

class diji::GameObject;
namespace pacman
{
	class PickUp;

		//class PickUpLoader final : public Singleton<PickUpLoader>
	class PickUpLoader final
	{
	public:
		PickUpLoader(const diji::GameObject* player, const std::vector<diji::GameObject*>& gameObjects);
		~PickUpLoader() = default;


		PickUpLoader(const PickUpLoader& other) = delete;
		PickUpLoader(PickUpLoader&& other) = delete;
		PickUpLoader& operator=(const PickUpLoader& other) = delete;
		PickUpLoader& operator=(PickUpLoader&& other) = delete;

		//void Initialize(diji::GameObject* player);

		void AddPickUp(const std::string& file, const int width, const int height, const glm::vec2& pos, const int value, const diji::GameObject* pelletCounter);
		void AddPowerUp(const std::vector<diji::GameObject*>& gameObjects, const std::string& file, const int width, const int height, const glm::vec2& pos, const int value);
	private:
		//friend class Singleton<PickUpLoader>;
		//PickUpLoader() = default;
		std::vector<std::vector<glm::vec2>> m_PelletsVec;

		diji::Scene* m_ScenePtr = nullptr;
		const diji::GameObject* m_PlayerPtr = nullptr;
	};
}
