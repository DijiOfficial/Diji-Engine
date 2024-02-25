#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

unsigned int diji::Scene::m_idCounter = 0;

diji::Scene::Scene(const std::string& name) : m_name(name) {}

diji::Scene::~Scene() = default;

void diji::Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void diji::Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void diji::Scene::RemoveAll()
{
	m_objects.clear();
}

void diji::Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}
}

void diji::Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

