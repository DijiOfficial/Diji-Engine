#include "GameObject.h"

diji::GameObject::~GameObject()
{
};

void diji::GameObject::RemoveComponent(const Component& component)
{
	m_ComponentsPtrVec.erase(std::remove_if(m_ComponentsPtrVec.begin(), m_ComponentsPtrVec.end(),
		[&](const auto& comp) 
		{ 
			return comp.get() == &component;
		}),
		m_ComponentsPtrVec.end());
}

void diji::GameObject::Update()
{
	for (const auto& component : m_ComponentsPtrVec)
	{
		component->Update(*this);
	}
};

void diji::GameObject::Render() const
{
	for (const auto& component : m_ComponentsPtrVec)
	{
		component->Render();
	}
}

void diji::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}
