#include "Scene.h"

#include <algorithm>
#include <stdexcept>

diji::Scene::~Scene()
{
	m_ObjectsUPtrMap.clear();
};

diji::GameObject* diji::Scene::CreateGameObject(const std::string& name)
{
	if (m_ObjectsUPtrMap.find(name) != m_ObjectsUPtrMap.end())
		throw std::runtime_error("A GameObject with the given name already exists.");
	
	m_ObjectsUPtrMap[name] = (std::make_unique<GameObject>());
	return m_ObjectsUPtrMap.at(name).get();
}

void diji::Scene::Remove(GameObject* object)
{
	for (auto it = m_ObjectsUPtrMap.begin(); it != m_ObjectsUPtrMap.end(); ++it)
	{
		if (it->second.get() == object)
		{
			m_ObjectsUPtrMap.erase(it);
			break;
		}
	}
}

void diji::Scene::Remove(const std::string& name)
{
	auto it = m_ObjectsUPtrMap.find(name);
	if (it != m_ObjectsUPtrMap.end())
	{
		m_ObjectsUPtrMap.erase(it);
	}
}

void diji::Scene::RemoveAll()
{
	m_ObjectsUPtrMap.clear();
}

diji::GameObject* diji::Scene::GetGameObject(const std::string& name) const
{
	//todo: check if the object exists otherwise throw an exception
	return m_ObjectsUPtrMap.at(name).get();
}

std::unique_ptr<diji::GameObject> diji::Scene::RemoveAndReturnGameObject(const GameObject* object)
{
	for (auto it = m_ObjectsUPtrMap.begin(); it != m_ObjectsUPtrMap.end(); ++it)
	{
		if (it->second.get() == object)
		{
			auto removedObj = std::move(it->second);
			m_ObjectsUPtrMap.erase(it);
			return removedObj;
		}
	}
	return nullptr;
}

void diji::Scene::AddExistingGameObject(std::unique_ptr<GameObject> object, const std::string& name)
{
	if (m_ObjectsUPtrMap.find(name) != m_ObjectsUPtrMap.end())
	{
		throw std::runtime_error("A GameObject with the given name already exists in the target scene.");
	}
	m_ObjectsUPtrMap[name] = std::move(object);
}

std::string diji::Scene::GetGameObjectName(const GameObject* object) const
{
	for (const auto& pair : m_ObjectsUPtrMap)
	{
		if (pair.second.get() == object)
		{
			return pair.first;
		}
	}
	throw std::runtime_error("GameObject not found in the scene.");
}

void diji::Scene::Update()
{
	for(auto& object : m_ObjectsUPtrMap)
	{
		object.second->Update();
	}
}

void diji::Scene::FixedUpdate()
{
	for (auto& object : m_ObjectsUPtrMap)
	{
		object.second->FixedUpdate();
	}
}

void diji::Scene::Init()
{
	for (const auto& object : m_ObjectsUPtrMap)
	{
		object.second->Init();
	}
}

void diji::Scene::Render() const
{
	for (const auto& object : m_ObjectsUPtrMap)
	{
		object.second->Render();
	}
}
