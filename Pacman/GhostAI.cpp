#include "GhostAI.h"
#include "Collider.h"
#include "Transform.h"
#include "Texture.h"
#include "Observers.h"
#include "GameObject.h"
#include "GhostStates.h"
#include "TimeSingleton.h"
#include "AI.h"
#include "GhostCollision.h"
#include "ISoundSystem.h"
#include "GhostsAlgorithm.h"

#pragma region GhostAI
pacman::GhostAI::GhostAI(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter, const diji::GameObject* timers)
	: Component(ownerPtr)
	, m_PlayerColliderPtr{ player->GetComponent<diji::Collider>() }
	, m_PlayerTransformPtr{ player->GetComponent<diji::Transform>() }
	, m_PelletCounterPtr{ pelletCounter->GetComponent<PelletObserver>() }
	, m_GhostsTimerPtr{ timers->GetComponent<GhostsTimers>() }
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
}

void pacman::GhostAI::Update()
{
	if (m_GhostsTimerPtr->IsPaused())
		return;
	
	//todo: magic mumber removal?
	const auto& pos = m_TransformCompPtr->GetPosition();
	m_CurrentStateUPtr->SetInTunnel(pos.y == 295 and (pos.x <= 90 or pos.x >= 360));
	
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
	if (m_GhostsTimerPtr->IsPaused() and
		dynamic_cast<const pacman::Respawn*>(m_CurrentStateUPtr.get()) == nullptr and
		dynamic_cast<const pacman::Eaten*>(m_CurrentStateUPtr.get()) == nullptr)
	{
		return;
	}

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

void pacman::GhostAI::OnNotify(diji::MessageTypes message, diji::Subject* subject)
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
	case MessageTypesDerived::ENEMY_COLLISION:
		if (not m_IsFrightened)
			break;

		m_GhostsTimerPtr->Pause();

		constexpr int EATEN_GHOST_POINTS = 200;
		const auto& ghost = GetOwner()->GetComponent<GhostCollision>();
		if (subject == ghost)
		{
			m_CurrentStateUPtr->OnExit(this);
			m_CurrentStateUPtr = std::make_unique<Dying>(static_cast<int>(std::pow(2, ghost->GetGhostsEaten()) * EATEN_GHOST_POINTS));
			m_CurrentStateUPtr->OnEnter(this);
		}
		
		break;
	}
}

bool pacman::GhostAI::GetIsInChaseState() const
{
	return m_GhostsTimerPtr->IsInChaseState();
};

int pacman::GhostAI::GetPelletCount() const
{
	return m_PelletCounterPtr->GetPelletCount();
}

void pacman::GhostAI::ClearFrightened() const
{
	m_IsFrightened = false;
	m_PowerUpTimer = 0.f; 
	//todo: this stops fright audio after eating only one ghost
	diji::ServiceLocator::GetSoundSystem().AddSoundRequest(diji::SoundId::Music, -1); 
}

void pacman::GhostAI::SetGhostTexture() const
{
	m_TextureCompPtr->SetTexture(m_TexturePath);
}

bool pacman::GhostAI::IsUpdatePaused() const
{
	return m_GhostsTimerPtr->IsPaused();
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
#pragma endregion
#pragma region Blinky
pacman::RedAI::RedAI(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter, const diji::GameObject* timers)
	: GhostAI(ownerPtr, player, pelletCounter, timers)
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
#pragma endregion
#pragma region Pinky
pacman::Pinky::Pinky(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter, const diji::GameObject* timers)
	: GhostAI(ownerPtr, player, pelletCounter, timers)
{
	m_PersonnalSpawn = { 212, 300 };
	m_ScatterTarget = { 0, 0 };
	m_TexturePath = "Pinky.png";

	m_CurrentStateUPtr = std::make_unique<Waiting>(7);
}

std::unique_ptr<pacman::GhostState> pacman::Pinky::GetChaseState() const
{
	return std::make_unique<PinkyChase>();
}

void pacman::Pinky::Init()
{
	GhostAI::Init();
	GetTransform()->SetMovement(diji::Movement::Up);
	m_CurrentStateUPtr->OnEnter(this);
}
#pragma endregion
pacman::Inky::Inky(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter, const diji::GameObject* timers, const diji::GameObject* blinky)
	: GhostAI(ownerPtr, player, pelletCounter, timers)
{
	m_PersonnalSpawn = { 180, 300 };
	m_ScatterTarget = { 432, 480 };
	m_TexturePath = "Inky.png";

	m_CurrentStateUPtr = std::make_unique<Waiting>(17);
	m_BlinkyTransformPtr = blinky->GetComponent<diji::Transform>();
}

std::unique_ptr<pacman::GhostState> pacman::Inky::GetChaseState() const
{
	return std::make_unique<pacman::InkyChase>();
}

void pacman::Inky::Init()
{
	GhostAI::Init();
	GetTransform()->SetMovement(diji::Movement::Up);
	m_CurrentStateUPtr->OnEnter(this);
}

pacman::Clyde::Clyde(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter, const diji::GameObject* timers)
	: GhostAI(ownerPtr, player, pelletCounter, timers)
{
	m_PersonnalSpawn = { 244, 300 };
	m_ScatterTarget = { 0, 480 };
	m_TexturePath = "Clyde.png";

	m_CurrentStateUPtr = std::make_unique<Waiting>(32);
}

std::unique_ptr<pacman::GhostState> pacman::Clyde::GetChaseState() const
{
	return std::make_unique<pacman::RedChase>();
}

void pacman::Clyde::Init()
{
	GhostAI::Init();
}
