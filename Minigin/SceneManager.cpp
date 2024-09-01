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

void diji::SceneManager::LateUpdate()
{
	m_ScenesUPtrMap.at(m_ActiveSceneId)->LateUpdate();
}

diji::Scene* diji::SceneManager::CreateScene(const int id)
{
	// Check if the scene already exists in the map
	auto it = m_ScenesUPtrMap.find(id);
	if (it != m_ScenesUPtrMap.end())
	{
		// Scene already exists, return the existing scene
		return it->second.get();
	}

	// Scene does not exist, create a new one and store it in the map
	m_ScenesUPtrMap[id] = std::make_unique<Scene>();
	return m_ScenesUPtrMap[id].get();
}