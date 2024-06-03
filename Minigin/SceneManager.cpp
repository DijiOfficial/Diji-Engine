#include "SceneManager.h"

void diji::SceneManager::Update()
{
	if (m_NextScene != m_ActiveSceneId)
	{
		m_ActiveSceneId = m_NextScene;
		m_ScenesUPtrMap.at(m_ActiveSceneId)->Init();
	}

	m_ScenesUPtrMap.at(m_ActiveSceneId)->Update();

	if (m_SceneTransferInfoVec.empty())
		return;

	for (const auto& transferInfo : m_SceneTransferInfoVec)
	{
		const std::string& name = m_ScenesUPtrMap.at(transferInfo.fromScene)->GetGameObjectName(transferInfo.object);
		std::unique_ptr<GameObject> removedObj = m_ScenesUPtrMap.at(transferInfo.fromScene)->RemoveAndReturnGameObject(transferInfo.object);
		if (removedObj)
		{
			m_ScenesUPtrMap.at(transferInfo.toScene)->AddExistingGameObject(std::move(removedObj), name);
		}
	}
	m_SceneTransferInfoVec.clear();
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

#include "Renderer.h"
void diji::SceneManager::Render()
{
	m_ScenesUPtrMap.at(m_ActiveSceneId)->Render();
	//for (const auto& scene : m_ScenesUPtrVec)
	//{
	//	scene->Render();
	//}
	//temp
	//for (int i{}; i < (28 * 34); ++i)
	//{
	//	Renderer::GetInstance().DrawRect(Rectf{2+ static_cast<float>(i % 28) * 16, 78 + static_cast<float>((i / 28)) * 16, 16.f, 16.f }, SDL_Color{128,128, 0,255 });
	//}
	diji::Renderer::GetInstance().DrawCircle(0, 0, 5);
}

diji::Scene* diji::SceneManager::GetScene(const int id) const
{
	return m_ScenesUPtrMap.at(id).get();
}

diji::Scene* diji::SceneManager::CreateScene(const int id)
{
	// Create a new Scene and overwrite any existing one
	m_ScenesUPtrMap[id] = std::make_unique<Scene>();
	return m_ScenesUPtrMap[id].get();
}

//diji::Scene* diji::SceneManager::CreateScene(const int id)
//{
//	auto it = m_ScenesUPtrMap.find(id);
//	if (it != m_ScenesUPtrMap.end())
//	{
//		m_ScenesUPtrMap.erase(it);
//	}
//
//	m_ScenesUPtrMap[id] = std::make_unique<Scene>();
//	return m_ScenesUPtrMap[id].get();
//}

void diji::SceneManager::TranferScene(int fromScene, int toScene, const GameObject* object)
{
	m_SceneTransferInfoVec.push_back(SceneTransferInfo{ fromScene, toScene, object });
}