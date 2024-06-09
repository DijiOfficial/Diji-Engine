#pragma once
#include "GameObject.h"
#include <map>
#include <string>
namespace diji 
{
	class Scene final
	{
	public:
		Scene() = default;
		~Scene() noexcept;

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void Update();
		void FixedUpdate();
		void Init();
		void Render() const;
		void LateUpdate();
		
		GameObject* CreateGameObject(const std::string& name);
		void Remove(GameObject* object);
		void Remove(const std::string& name);
		void RemoveAll();
		GameObject* GetGameObject(const std::string& name) const;
		std::string GetGameObjectName(const GameObject* object) const;
		
	private:
		std::map<std::string, std::unique_ptr<GameObject>> m_ObjectsUPtrMap;
	};
}