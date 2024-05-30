#include "SceneManager.h"

void diji::SceneManager::Update()
{
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
	auto it = m_ScenesUPtrMap.find(id);
	assert(it == m_ScenesUPtrMap.end() && "Scene with the given ID already exists!");

	m_ScenesUPtrMap[id] = std::make_unique<Scene>();
	return m_ScenesUPtrMap[id].get();
}
