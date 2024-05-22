#include "SceneManager.h"

diji::SceneManager::SceneManager()
{
}

diji::SceneManager::~SceneManager()
{
}

void diji::SceneManager::Update()
{
	for(auto& scene : m_ScenesUPtrVec)
	{
		scene->Update();
	}
}

void diji::SceneManager::FixedUpdate()
{
	for (auto& scene : m_ScenesUPtrVec)
	{
		scene->FixedUpdate();
	}
}

void diji::SceneManager::Init()
{
for (auto& scene : m_ScenesUPtrVec)
	{
		scene->Init();
	}
}

#include "Renderer.h"
void diji::SceneManager::Render()
{
	for (const auto& scene : m_ScenesUPtrVec)
	{
		scene->Render();
	}
	//temp
	//for (int i{}; i < (28 * 34); ++i)
	//{
	//	Renderer::GetInstance().DrawRect(Rectf{2+ static_cast<float>(i % 28) * 16, 78 + static_cast<float>((i / 28)) * 16, 16.f, 16.f }, SDL_Color{128,128, 0,255 });
	//}
	Renderer::GetInstance().DrawCircle(450, 60, 5);
}

diji::Scene* diji::SceneManager::GetScene(std::string& name) const
{
	for (const auto& scenePtr : m_ScenesUPtrVec)
	{
		if (scenePtr->GetName() == name)
		{
			return scenePtr.get();
		}
	}
	return nullptr;
}

diji::Scene* diji::SceneManager::CreateScene(const std::string& name)
{
	m_ScenesUPtrVec.emplace_back(std::make_unique<Scene>(name));
	return m_ScenesUPtrVec.back().get();
}
