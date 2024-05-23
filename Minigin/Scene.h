#pragma once
#include <vector>
#include <string>
#include <memory>

namespace diji 
{
	class GameObject;
	class Scene final
	{
	public:
		explicit Scene(const std::string& name);
		~Scene() noexcept;

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void Update();
		void FixedUpdate();
		void Init();
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