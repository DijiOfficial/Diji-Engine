#include "GameObject.h"

#include "Render.h"
#include "Transform.h"
#include <algorithm>

void diji::GameObject::Update()
{
    for (const auto& component : m_ComponentsPtrVec)
    {
        component->Update();
    }
    if (m_TransformCompPtr)
        UpdateWorldPosition();
}

void diji::GameObject::FixedUpdate()
{
    for (const auto& component : m_ComponentsPtrVec)
    {
		component->FixedUpdate();
	}
}

void diji::GameObject::Init()
{
	for (const auto& component : m_ComponentsPtrVec)
	{
		component->Init();
	}
};

void diji::GameObject::Render() const
{
    if (m_RenderCompPtr) m_RenderCompPtr->RenderFrame();
}

void diji::GameObject::RemoveComponent(const Component& component)
{
	m_ComponentsPtrVec.erase(std::remove_if(m_ComponentsPtrVec.begin(), m_ComponentsPtrVec.end(),
		[&](const auto& comp) 
		{ 
			return comp.get() == &component;
		}),
		m_ComponentsPtrVec.end());
}

void diji::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
    if (parent == m_ParentPtr || parent == this || IsChildOf(parent))
        return;

    if (parent)
    {
        if (keepWorldPosition)
            SetLocalPosition(m_LocalPosition - parent->GetWorldPosition());
        SetPositionDirty();
    }
    else
        SetLocalPosition(GetWorldPosition());

    if (m_ParentPtr)
        m_ParentPtr->RemoveChild(this);

    m_ParentPtr = parent;
    
    if (m_ParentPtr)
        m_ParentPtr->AddChild(this);    
}

void diji::GameObject::AddChild(GameObject* child)
{
    if (child == nullptr || child->IsChildOf(this))
        return;

    if (child->m_ParentPtr)
    {
        child->m_ParentPtr->RemoveChild(child);
    }

    child->m_ParentPtr = this;

    m_ChildrenPtrVec.push_back(child);

    child->UpdateWorldPosition();
}

void diji::GameObject::RemoveChild(GameObject* child)
{
    auto it = std::find(m_ChildrenPtrVec.begin(), m_ChildrenPtrVec.end(), child);
    if (child == nullptr || it == m_ChildrenPtrVec.end())
        return;

    m_ChildrenPtrVec.erase(it);
    
    child->SetLocalPosition(GetWorldPosition() - child->m_LocalPosition); //local position now becomes world position
    child->SetPositionDirty();

    child->m_ParentPtr = nullptr;
}

void diji::GameObject::SetLocalPosition(const glm::vec3& pos)
{
    m_LocalPosition = pos;
    SetPositionDirty();
}

const glm::vec3& diji::GameObject::GetWorldPosition()
{
    if (m_PositionIsDirty)
        UpdateWorldPosition();

    return m_TransformCompPtr->GetPosition();
}

void diji::GameObject::SetPositionDirty()
{
	m_PositionIsDirty = true;
	for (const auto& childPtr : m_ChildrenPtrVec)
	{
        if (not childPtr->m_PositionIsDirty)
            childPtr->SetPositionDirty();
	}
}

void diji::GameObject::UpdateWorldPosition()
{
    if (not m_PositionIsDirty)
        return;

    if (m_ParentPtr)
        m_TransformCompPtr->SetPosition(m_ParentPtr->GetWorldPosition() + m_LocalPosition);
    else
        m_TransformCompPtr->SetPosition(m_LocalPosition);
    
    m_PositionIsDirty = false;
}

bool diji::GameObject::IsChildOf(GameObject* potentialChild) const
{
    return std::any_of(m_ChildrenPtrVec.begin(), m_ChildrenPtrVec.end(),
        [&](GameObject* child)
        {
            if (child == potentialChild)
                return true;
            else
                return child->IsChildOf(potentialChild);
        });
}
