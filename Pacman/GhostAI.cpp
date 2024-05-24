#include "GhostAI.h"
#include "Collider.h"
#include "Transform.h"
#include "Texture.h"
#include "Observers.h"
#include "GameObject.h"
#include "GhostStates.h"
#include "TimeSingleton.h"
#include "AI.h"

pacman::GhostAI::GhostAI(diji::GameObject* ownerPtr, diji::GameObject* player)
	: Component(ownerPtr)
	, m_PlayerColliderPtr{ player->GetComponent<diji::Collider>() }
{
	m_ColliderCompPtr = nullptr;
	m_TransformCompPtr = nullptr;
	m_TextureCompPtr = nullptr;

}

void pacman::GhostAI::Init()
{
	const auto& ownerPtr = GetOwner();
	m_ColliderCompPtr = ownerPtr->GetComponent<diji::Collider>();
	m_TransformCompPtr = ownerPtr->GetComponent<diji::Transform>();
	m_TextureCompPtr = ownerPtr->GetComponent<diji::Texture>();

	m_TransformCompPtr->SetMovement(diji::Movement::Up);
}

void pacman::GhostAI::Update()
{
	m_ChaseScatterAlgo->Update();

	if (m_IsFrightened)
	{
		m_PowerUpTimer += diji::TimeSingleton::GetInstance().GetDeltaTime();
		if (m_PowerUpTimer >= 10.f)
		{
			m_IsFrightened = false;
			m_PowerUpTimer = 0.f;
		}
	}
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
		m_IsFrightened = true;
		m_PowerUpTimer = 0.f;

		m_TextureCompPtr->SetNrOfFrames(2);
		m_TextureCompPtr->DisableFlickerAnimation();

		if (auto frightenedState = dynamic_cast<Frightened*>(m_CurrentStateUPtr.get()))
			frightenedState->ResetUpdate();

		break;
	}
}

void pacman::GhostAI::ClearFrightened() const
{
	m_IsFrightened = false;
	m_PowerUpTimer = 0.f; 
}

void pacman::GhostAI::SetGhostTexture() const
{
	m_TextureCompPtr->SetTexture(m_TexturePath);
}

void pacman::GhostAI::TurnAround() const
{
	m_TransformCompPtr->SetMovement(static_cast<diji::Movement>((static_cast<int>(m_TransformCompPtr->GetMovement()) + 2) % 4));
}

//const diji::Rectf pacman::GhostAI::CalculateNewPosition(diji::Movement movement, const GhostAI* ghost)
//{
//	auto shape = m_ColliderCompPtr->GetCollisionBox() + ghost->GetTransform()->GetMovementVector(2.f);
//
//	if (diji::Collision::GetInstance().IsCollidingWithWorld(shape))
//	{
//		shape.left = std::round(shape.left);
//		shape.bottom = std::round(shape.bottom);
//		switch (movement)
//		{
//		case diji::Movement::Up:
//			++shape.bottom;
//			break;
//		case diji::Movement::Down:
//			--shape.bottom;
//			break;
//		case diji::Movement::Left:
//			++shape.left;
//			break;
//		case diji::Movement::Right:
//			--shape.left;
//			break;
//		}
//	}
//
//	return shape;
//}

pacman::RedAI::RedAI(diji::GameObject* ownerPtr, diji::GameObject* player)
	: GhostAI(ownerPtr, player)
{
	m_PersonnalSpawn = { 212, 300 };
	m_ScatterTarget = { 432, 0 };
	m_TexturePath = "RedGhost.png";

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