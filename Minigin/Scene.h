#pragma once
#include "SceneManager.h"

namespace diji 
{
	class GameObject;
	class Scene final
	{
	public:
		explicit Scene(const std::string& name);
		~Scene();

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void Update();
		void FixedUpdate();
		void Render() const;
		
		GameObject* CreateGameObject();
		void Remove(GameObject* object);
		void RemoveAll();

		std::string GetName() const { return m_Name; };
	
	private: 

		std::string m_Name;
		std::vector<std::unique_ptr<GameObject>> m_ObjectsUPtrVec;

		static unsigned int m_IdCounter; 
	};
}