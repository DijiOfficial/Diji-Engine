#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

unsigned int diji::Scene::m_IdCounter = 0;

diji::Scene::Scene(const std::string& name) : m_Name(name) {}

diji::Scene::~Scene() = default;

diji::GameObject* diji::Scene::CreateGameObject()
{
	m_ObjectsUPtrVec.emplace_back(std::make_unique<GameObject>());
	return m_ObjectsUPtrVec.back().get();
}

void diji::Scene::Remove(GameObject* object)
{
	std::erase_if(m_ObjectsUPtrVec,
		[object](const auto& objUPtr)
		{
			return objUPtr.get() == object;
		}
	);
}

void diji::Scene::RemoveAll()
{
	m_ObjectsUPtrVec.clear();
}

void diji::Scene::Update()
{
	for(auto& object : m_ObjectsUPtrVec)
	{
		object->Update();
	}
}

void diji::Scene::FixedUpdate()
{
	for (auto& object : m_ObjectsUPtrVec)
	{
		object->FixedUpdate();
	}
}

void diji::Scene::Render() const
{
	for (const auto& object : m_ObjectsUPtrVec)
	{
		object->Render();
	}
}
