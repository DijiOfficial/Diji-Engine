#include "GhostAI.h"
#include "Collider.h"
#include "Transform.h"

#include "Render.h"
diji::GhostAI::GhostAI(GameObject* ownerPtr, GameObject* player)
	: Component(ownerPtr)
	, m_PlayerColliderPtr{ player->GetComponent<Collider>() }
{
	m_TransformCompPtr = ownerPtr->GetComponent<Transform>();
	m_ColliderCompPtr = ownerPtr->GetComponent<Collider>();

	assert(m_TransformCompPtr and "AI Component needs to be initialized aftera Transform");
	assert(m_ColliderCompPtr and "AI Component needs to be initialized aftera Collider");

	m_TransformCompPtr->SetMovement(Movement::Up);
}

void diji::GhostAI::FixedUpdate()
{
	auto state = m_CurrentStateUPtr->Execute(m_TransformCompPtr, m_ColliderCompPtr, m_PlayerColliderPtr);

	if (state)
	{
		m_CurrentStateUPtr->OnExit(this);
		m_CurrentStateUPtr = std::move(state);
		m_CurrentStateUPtr->OnEnter(this);
	}
}