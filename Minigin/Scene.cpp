#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

unsigned int diji::Scene::m_IdCounter = 0;

diji::Scene::Scene(const std::string& name) : m_Name(name) {}

diji::Scene::~Scene() = default;

void diji::Scene::Add(std::shared_ptr<GameObject> object)
{
	m_ObjectsPtrVec.emplace_back(std::move(object));
}

void diji::Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_ObjectsPtrVec.erase(std::remove(m_ObjectsPtrVec.begin(), m_ObjectsPtrVec.end(), object), m_ObjectsPtrVec.end());
}

void diji::Scene::RemoveAll()
{
	m_ObjectsPtrVec.clear();
}

void diji::Scene::Update()
{
	for(auto& object : m_ObjectsPtrVec)
	{
		object->Update();
	}
}

void diji::Scene::Render() const
{
	for (const auto& object : m_ObjectsPtrVec)
	{
		object->Render();
	}
}

