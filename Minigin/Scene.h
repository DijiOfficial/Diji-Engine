#pragma once
#include "SceneManager.h"

namespace diji 
{
	class GameObject;

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		~Scene();

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void Update();
		void Render() const;
		
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();
		std::vector < std::shared_ptr<GameObject>> GetObjects() const { return m_ObjectsPtrVec; };

	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector < std::shared_ptr<GameObject>> m_ObjectsPtrVec{};

		static unsigned int m_IdCounter; 
	};
}