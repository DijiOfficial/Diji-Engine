#include "SceneManager.h"
#include "Scene.h"

void diji::SceneManager::Update()
{
	for(auto& scene : m_ScenesPtrVec)
	{
		scene->Update();
	}
}

void diji::SceneManager::Render()
{
	for (const auto& scene : m_ScenesPtrVec)
	{
		scene->Render();
	}
}

diji::Scene& diji::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_ScenesPtrVec.push_back(scene);
	return *scene;
}
