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
#include "Render.h"

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
	m_NextDirection = diji::Movement::Left;
}

void pacman::GhostAI::Reset()
{
	m_CurrentStateUPtr->OnExit(this);
	const int respawnPellets = m_PelletCounterPtr->GetPelletCount() + m_PelletsNeededForRespawn;
	m_CurrentStateUPtr = std::make_unique<Waiting>(respawnPellets);
	m_TransformCompPtr->SetPosition(m_PersonnalSpawn);
	if (m_TexturePath == "RedGhost.png")
	{
		constexpr glm::ivec2 BLINKY_SPAWN = { 212, 247 };
		m_TransformCompPtr->SetPosition(BLINKY_SPAWN);
		if (m_IsInFreeMode) 
		{
			m_TransformCompPtr->SetMovement(diji::Movement::Left);
			m_CurrentStateUPtr = std::make_unique<RedChase>();
		}
		else
		{
			m_TransformCompPtr->SetMovement(diji::Movement::Right);
			m_CurrentStateUPtr = std::make_unique<Scatter>();
		}
		
	}
	m_TextureCompPtr->SetTexture(m_TexturePath);
	m_PowerUpTimer = 0.f;
	m_IsFrightened = false;
	m_IsLastGhostEaten = false;
	m_PlayerDeathTimer = 0.f;
	m_IsPlayerKilled = false;
	m_GhostsTimerPtr->Pause();
	m_CurrentStateUPtr->OnEnter(this);
	GetOwner()->GetComponent<diji::Render>()->EnableRender();
	//Reset the collider to avoid collision in the next frame
	m_ColliderCompPtr->Update();
}

void pacman::GhostAI::Init()
{
	const auto& ownerPtr = GetOwner();
	m_ColliderCompPtr = ownerPtr->GetComponent<diji::Collider>();
	m_TransformCompPtr = ownerPtr->GetComponent<diji::Transform>();
	m_TextureCompPtr = ownerPtr->GetComponent<diji::Texture>();
	const auto movement = GetIsInMenu() ? diji::Movement::Right : diji::Movement::Up;
	m_TransformCompPtr->SetMovement(movement);
}

void pacman::GhostAI::Update()
{
	if (m_IsPlayerKilled)
	{
		constexpr float TOTAL_WAIT = 2.54f;
		m_PlayerDeathTimer += diji::TimeSingleton::GetInstance().GetDeltaTime();
		if (m_PlayerDeathTimer >= 1.f)
			GetOwner()->GetComponent<diji::Render>()->DisableRender();

		if (m_PlayerDeathTimer < TOTAL_WAIT)
		 return;

		for (const auto& ghost : m_GhostsPtrs)
		{
			ghost->Reset();
		}
		m_GhostsTimerPtr->Pause();
	}

	if (m_GhostsTimerPtr->IsPaused())
		return;
	
	constexpr float TUNNEL_Y_POSITION = 295.0f;
	constexpr float TUNNEL_X_MIN = 90.0f;
	constexpr float TUNNEL_X_MAX = 360.0f;
	constexpr float POWER_UP_DURATION = 10.0f;

	glm::vec3 pos = m_TransformCompPtr->GetPosition();
	if (m_IsInFreeMode)
		pos.y = floor(pos.y);
	
	m_CurrentStateUPtr->SetInTunnel(pos.y == TUNNEL_Y_POSITION and (pos.x <= TUNNEL_X_MIN or pos.x >= TUNNEL_X_MAX));

	if (m_IsFrightened)
	{
		m_PowerUpTimer += diji::TimeSingleton::GetInstance().GetDeltaTime();
		if (m_PowerUpTimer >= POWER_UP_DURATION)
		{
			m_IsFrightened = false;
			m_PowerUpTimer = 0.0f;
			diji::ServiceLocator::GetSoundSystem().AddSoundRequest("siren_1.wav", true, -1);
		}
	}
}

void pacman::GhostAI::FixedUpdate()
{
	if (m_IsPlayerKilled)
		return;

	if (m_GhostsTimerPtr->IsPaused() and
		dynamic_cast<const pacman::Respawn*>(m_CurrentStateUPtr.get()) == nullptr and
		dynamic_cast<const pacman::Eaten*>(m_CurrentStateUPtr.get()) == nullptr)
	{
		return;
	}

	auto state = m_CurrentStateUPtr->Execute(this);
	
	//no use of ternary to avoid dynamic casting when not in menu
	bool reset = false;
	if (m_IsInMenu and dynamic_cast<Eaten*>(state.get()) != nullptr)
		reset = true;

	if (state)
	{
		m_CurrentStateUPtr->OnExit(this);
		m_CurrentStateUPtr = std::move(state);
		m_CurrentStateUPtr->OnEnter(this);
	}

	if (reset and m_IsLastGhostEaten)
		Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::RESET_MENU));
}

void pacman::GhostAI::LateUpdate()
{
	if (m_IsInFreeMode)
	{
		const auto& shape = m_ColliderCompPtr->GetCollisionBox();
		if (!m_TeleportedThisFrame)
		{
			if (shape.left < 0 - shape.width)
			{
				m_TransformCompPtr->SetPosition(AI::TOTAL_WIDTH, shape.bottom);
				m_TeleportedThisFrame = true;
			}
			else if (shape.left > AI::TOTAL_WIDTH)
			{
				m_TransformCompPtr->SetPosition(0 - shape.width, shape.bottom);
				m_TeleportedThisFrame = true;
			}
		}
		else
		{
			m_TeleportedThisFrame = false;
		}
	}
	else
	{
		auto currentMovement = m_TransformCompPtr->GetMovement();
		auto shape = m_ColliderCompPtr->GetCollisionBox();
		if (currentMovement == diji::Movement::Left)
			if (shape.left < 0 - shape.width)
				m_TransformCompPtr->SetPosition(AI::TOTAL_WIDTH, shape.bottom);
		if (currentMovement == diji::Movement::Right)
			if (shape.left > AI::TOTAL_WIDTH)
				m_TransformCompPtr->SetPosition(0 - shape.width, shape.bottom);
	}
}

void pacman::GhostAI::OnNotify(diji::MessageTypes message, diji::Subject* subject)
{
	auto msg = static_cast<MessageTypesDerived>(message);
	switch (msg)
	{
	case MessageTypesDerived::LEVEL_END:
		Reset();
		break;
	case MessageTypesDerived::LEVEL_BEGIN:
		GetOwner()->GetComponent<diji::Render>()->EnableRender();
		break;

	case MessageTypesDerived::POWERUP_COLLISION:
		m_IsFrightened = true;
		m_PowerUpTimer = 0.f;

		m_TextureCompPtr->SetNrOfFrames(2);
		m_TextureCompPtr->DisableFlickerAnimation();

		if (auto frightenedState = dynamic_cast<Frightened*>(m_CurrentStateUPtr.get()))
			frightenedState->ResetUpdate();

		break;
	case MessageTypesDerived::ENEMY_COLLISION:
		if (m_IsPlayerKilled)
			break;

		if (not m_IsFrightened)
		{
			for (const auto& ghost : m_GhostsPtrs)
			{
				ghost->SetGhostKilledPlayer();
			}
			break;
		}

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

void pacman::GhostAI::SetInMenu()
{
	m_IsInMenu = true;
	m_CurrentStateUPtr = GetChaseState();
};

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
#pragma endregion
#pragma region Blinky
pacman::RedAI::RedAI(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter, const diji::GameObject* timers)
	: GhostAI(ownerPtr, player, pelletCounter, timers)
{
	m_PersonnalSpawn = { 212, 300 };
	m_ScatterTarget = { 432, 0 };
	m_TexturePath = "RedGhost.png";

	m_PelletsNeededForRespawn = 0;
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
	m_PelletsNeededForRespawn = 7;

	m_CurrentStateUPtr = std::make_unique<Waiting>(m_PelletsNeededForRespawn);
}

std::unique_ptr<pacman::GhostState> pacman::Pinky::GetChaseState() const
{
	return std::make_unique<PinkyChase>();
}

void pacman::Pinky::Init()
{
	GhostAI::Init();
	m_CurrentStateUPtr->OnEnter(this);
}
#pragma endregion
#pragma region Inky
pacman::Inky::Inky(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter, const diji::GameObject* timers, const diji::GameObject* blinky)
	: GhostAI(ownerPtr, player, pelletCounter, timers)
{
	m_PersonnalSpawn = { 180, 300 };
	m_ScatterTarget = { 432, 480 };
	m_TexturePath = "Inky.png";
	m_PelletsNeededForRespawn = 17;

	m_CurrentStateUPtr = std::make_unique<Waiting>(m_PelletsNeededForRespawn);
	m_BlinkyTransformPtr = blinky->GetComponent<diji::Transform>();
}

std::unique_ptr<pacman::GhostState> pacman::Inky::GetChaseState() const
{
	return std::make_unique<pacman::InkyChase>();
}

void pacman::Inky::Init()
{
	GhostAI::Init();
	m_CurrentStateUPtr->OnEnter(this);
}
#pragma endregion
#pragma region Clyde
pacman::Clyde::Clyde(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter, const diji::GameObject* timers)
	: GhostAI(ownerPtr, player, pelletCounter, timers)
{
	m_PersonnalSpawn = { 244, 300 };
	m_ScatterTarget = { 0, 480 };
	m_TexturePath = "Clyde.png";
	m_PelletsNeededForRespawn = 32;

	m_CurrentStateUPtr = std::make_unique<Waiting>(m_PelletsNeededForRespawn);
}

std::unique_ptr<pacman::GhostState> pacman::Clyde::GetChaseState() const
{
	return std::make_unique<pacman::ClydeChase>();
}

void pacman::Clyde::Init()
{
	GhostAI::Init();
	m_CurrentStateUPtr->OnEnter(this);
}
#pragma endregion