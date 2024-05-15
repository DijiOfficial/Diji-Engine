#include "Scene.h"

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

void diji::SceneManager::Render()
{
	for (const auto& scene : m_ScenesUPtrVec)
	{
		scene->Render();
	}
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
