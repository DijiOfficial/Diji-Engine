#pragma once
#include "GameObject.h"

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
		
		GameObject* CreateGameObject();
		void Remove(GameObject* object);
		void RemoveAll();
	
	private:
		std::vector<std::unique_ptr<GameObject>> m_ObjectsUPtrVec;
	};
}