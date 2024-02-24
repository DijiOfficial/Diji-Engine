#include "GameObject.h"

GameObject::~GameObject()
{
};

void GameObject::RemoveComponent(const Component& component)
{
	m_ComponentsPtrVec.erase(std::remove_if(m_ComponentsPtrVec.begin(), m_ComponentsPtrVec.end(),
		[&](const auto& comp) 
		{ 
			return comp.get() == &component;
		}),
		m_ComponentsPtrVec.end());
}

void GameObject::Update()
{
	for (const auto& component : m_ComponentsPtrVec)
	{
		component->Update(*this);
	}
};

void GameObject::Render() const
{
	for (const auto& component : m_ComponentsPtrVec)
	{
		component->Render();
	}
}

void GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
