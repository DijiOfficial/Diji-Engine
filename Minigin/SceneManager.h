#pragma once
#include "Singleton.h"
#include "Scene.h"
#include <map>

namespace diji
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene* CreateScene(const int id);
		Scene* GetScene(const int id) const;
		
		void Update();
		void FixedUpdate();
		void Init();
		void Render();

		void SetActiveScene(const int id) { m_ActiveSceneId = id; m_NextScene = id; };
		void SetNextSceneToActivate(const int id) { m_NextScene = id; };

	private:
		std::map<int, std::unique_ptr<Scene>> m_ScenesUPtrMap;
		int m_ActiveSceneId = 0;
		int m_NextScene = 0;
	};
}