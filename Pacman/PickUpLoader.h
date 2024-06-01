#pragma once
#include "Scene.h"
#include <glm/glm.hpp>
#include <string>

class diji::GameObject;
namespace pacman
{
	class PickUp;

	class PickUpLoader final
	{
	public:
		PickUpLoader(const diji::GameObject* player, const std::vector<diji::GameObject*>& gameObjects, const diji::GameObject* pelletCounter);
		~PickUpLoader() noexcept = default;


		PickUpLoader(const PickUpLoader& other) = delete;
		PickUpLoader(PickUpLoader&& other) = delete;
		PickUpLoader& operator=(const PickUpLoader& other) = delete;
		PickUpLoader& operator=(PickUpLoader&& other) = delete;

		void AddPickUp(const std::string& file, const int width, const int height, const glm::vec2& pos, const int value, const diji::GameObject* pelletCounter);
		void AddPowerUp(const std::vector<diji::GameObject*>& gameObjects, const std::string& file, const int width, const int height, const glm::vec2& pos, const int value, const diji::GameObject* pelletCounter);
	private:
		std::vector<std::vector<glm::vec2>> m_PelletsVec;

		diji::Scene* m_ScenePtr = nullptr;
		const diji::GameObject* m_PlayerPtr = nullptr;
	};
}
