#pragma once
#include "Singleton.h"
#include "Scene.h"

namespace diji
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		~SceneManager() override;

		Scene* CreateScene(const std::string& name);

		void Update();
		void FixedUpdate();
		void Init();
		void Render();

		Scene* GetScene(std::string& name) const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager();

		std::vector<std::unique_ptr<Scene>> m_ScenesUPtrVec;
	};
}