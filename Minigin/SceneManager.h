#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace diji
{
	class Scene;

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		~SceneManager() override;

		Scene* CreateScene(const std::string& name);

		void Update();
		void Render();

		Scene* GetScene(std::string& name) const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager();

		std::vector<std::unique_ptr<Scene>> m_ScenesUPtrVec;
	};
}