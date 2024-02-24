//#include <string>
#include "GameObject.h"
//#include "ResourceManager.h"
//#include "Renderer.h"

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
		component->Update();
		//component->Update(m_ComponentsPtrVec);
	}
};

void GameObject::Render() const
{
	const auto& pos = m_transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

void GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
