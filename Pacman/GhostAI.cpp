#include "GhostAI.h"
#include "Collider.h"
#include "Transform.h"
#include "Texture.h"
#include "AI.h"
#include "Observers.h"
#include "GameObject.h"
#include "GhostStates.h"

pacman::GhostAI::GhostAI(diji::GameObject* ownerPtr, diji::GameObject* player)
	: Component(ownerPtr)
	, m_PlayerColliderPtr{ player->GetComponent<diji::Collider>() }
{
	m_TransformCompPtr = nullptr;
	m_ColliderCompPtr = nullptr;
	m_TextureCompPtr = nullptr;

	m_PlayerAICompPtr = player->GetComponent<AI>();
}

void pacman::GhostAI::Init()
{
	const auto& ownerPtr = GetOwner();
	m_TransformCompPtr = ownerPtr->GetComponent<diji::Transform>();
	m_ColliderCompPtr = ownerPtr->GetComponent<diji::Collider>();
	m_TextureCompPtr = ownerPtr->GetComponent<diji::Texture>();

	m_TransformCompPtr->SetMovement(diji::Movement::Up);
}

void pacman::GhostAI::FixedUpdate()
{
	auto state = m_CurrentStateUPtr->Execute(this);

	if (state)
	{
		m_CurrentStateUPtr->OnExit(this);
		m_CurrentStateUPtr = std::move(state);
		m_CurrentStateUPtr->OnEnter(this);
	}

	//todo: late Update
	auto currentMovement = m_TransformCompPtr->GetMovement();
	auto shape = m_ColliderCompPtr->GetCollisionBox();
	if (currentMovement == diji::Movement::Left)
		if (shape.left < 0 - shape.width)
			m_TransformCompPtr->SetPosition(AI::TOTAL_WIDTH, shape.bottom);
	if (currentMovement == diji::Movement::Right)
		if (shape.left > AI::TOTAL_WIDTH)
			m_TransformCompPtr->SetPosition(0 - shape.width, shape.bottom);
}

void pacman::GhostAI::OnNotify(diji::MessageTypes message, diji::Subject*)
{
	auto msg = static_cast<MessageTypesDerived>(message);
	switch (msg)
	{
	case MessageTypesDerived::POWERUP_COLLISION:
		if (m_LockPowerUp)
			m_LockPowerUp = false;
		break;
	}
}

bool pacman::GhostAI::GetIsPoweredUp() const
{
	return m_PlayerAICompPtr->GetIsPoweredUp();
}

void pacman::GhostAI::TurnAround() const
{
	m_TransformCompPtr->SetMovement(static_cast<diji::Movement>((static_cast<int>(m_TransformCompPtr->GetMovement()) + 2) % 4));
}

pacman::RedAI::RedAI(diji::GameObject* ownerPtr, diji::GameObject* player)
	: GhostAI(ownerPtr, player)
{
	m_PersonnalSpawn = { 212, 300 };
	m_ScatterTarget = { 450, 60 };

	m_CurrentStateUPtr = std::make_unique<Scatter>();
}

std::unique_ptr<pacman::GhostState> pacman::RedAI::GetChaseState() const
{
	return std::make_unique<RedChase>();
};

void pacman::RedAI::Init()
{
	GhostAI::Init();
	GetTransform()->SetMovement(diji::Movement::Right);
	m_CurrentStateUPtr->OnEnter(this);
}