#include "SceneManager.h"

void diji::SceneManager::Update()
{
	if (m_NextScene != m_ActiveSceneId)
	{
		m_ActiveSceneId = m_NextScene;
		m_ScenesUPtrMap.at(m_ActiveSceneId)->Init();
	}

	m_ScenesUPtrMap.at(m_ActiveSceneId)->Update();
}

void diji::SceneManager::FixedUpdate()
{
	m_ScenesUPtrMap.at(m_ActiveSceneId)->FixedUpdate();
}

void diji::SceneManager::Init()
{
	for (auto& scene : m_ScenesUPtrMap)
	{
		scene.second->Init();
	}
}

void diji::SceneManager::Render()
{
	m_ScenesUPtrMap.at(m_ActiveSceneId)->Render();
}

diji::Scene* diji::SceneManager::CreateScene(const int id)
{
	m_ScenesUPtrMap[id] = std::make_unique<Scene>();
	return m_ScenesUPtrMap[id].get();
}