#include "GameObject.h"
#include "Render.h"
#include "Transform.h"
#include <algorithm>

diji::GameObject::~GameObject()
{
};

void diji::GameObject::Update()
{
    if (not m_TransformCompPtr)
        m_TransformCompPtr = GetComponent<Transform>();

    if (not m_RenderCompPtr)
        m_RenderCompPtr = GetComponent<diji::Render>();

    if(GetWorldPosition() != m_LocalPosition)
		SetPositionDirty();

    for (const auto& component : m_ComponentsPtrVec)
    {
        component->Update();
    }
};

void diji::GameObject::Render() const
{
    m_RenderCompPtr->RenderFrame();
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

    if (child->m_ParentPtr and child->m_ParentPtr != this)
    {
        child->m_ParentPtr->RemoveChild(child);
    }

    if (child->m_ParentPtr != this)
        child->m_ParentPtr = this;

    m_ChildrenPtrVec.push_back(child);

    // Update position, rotation, and scale (not implemented here)
    // UpdatePositionRotationScale();
    //child->SetLocalPosition(m_LocalPosition - m_ParentPtr->GetWorldPosition());
    //UpdateWorldPosition();

}

void diji::GameObject::RemoveChild(GameObject* child)
{
    auto it = std::find(m_ChildrenPtrVec.begin(), m_ChildrenPtrVec.end(), child);
    if (child == nullptr || it == m_ChildrenPtrVec.end())
        return;

    m_ChildrenPtrVec.erase(it);

    child->m_ParentPtr = nullptr;

    // Update position, rotation, and scale (not implemented here)
    // UpdatePositionRotationScale();
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

    return m_TransformCompPtr ? m_TransformCompPtr->GetPosition() : m_LocalPosition;
}

void diji::GameObject::SetPositionDirty()
{
    //no intialize so make sure to set the local position before calling this function
    if (m_LocalPosition == glm::vec3{ 0, 0, 0 })
    {
        m_LocalPosition = GetWorldPosition();
        return;
    }

	m_PositionIsDirty = true;
	for (const auto& childPtr : m_ChildrenPtrVec)
	{
        if (not childPtr->m_PositionIsDirty)
            childPtr->SetPositionDirty();
	}
}

void diji::GameObject::UpdateWorldPosition()
{
    if (m_ParentPtr)
        m_TransformCompPtr->SetPosition(m_ParentPtr->GetWorldPosition() + m_LocalPosition);
    else
        SetLocalPosition(m_TransformCompPtr->GetPosition());
    
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
