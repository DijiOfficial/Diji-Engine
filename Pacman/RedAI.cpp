#include "RedAI.h"
#include "Command.h"

#include "Transform.h"
#include "Collision.h"
#include "Collider.h"
#include "TimeSingleton.h"

pacman::RedAI::RedAI(diji::GameObject* ownerPtr, diji::GameObject* player)
	: GhostAI(ownerPtr, player)
{
	m_PersonnalSpawn = { 212, 300 };
	m_ScatterTarget = { 450, 60 };

	m_CurrentStateUPtr = std::make_unique<Scatter>();
	GetTransform()->SetMovement(diji::Movement::Right);
	m_CurrentStateUPtr->OnEnter(this);
}