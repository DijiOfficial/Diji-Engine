#include "GhostStates.h"
#include "GhostAI.h"
#include "Collider.h"
#include "Collision.h"
#include "Command.h"
#include "Transform.h"
#include "Texture.h"
#include "ISoundSystem.h"

#pragma region GhostState
void pacman::GhostState::SeekTarget(const GhostAI* ghost, const glm::vec2& target)
{
	const auto& collider = ghost->GetCollider();
	auto shape = collider->GetCollisionBox();

	shape.left = std::round(shape.left);
	shape.bottom = std::round(shape.bottom);
	
	if (not m_TempLock)
	{
		const float speed = m_IsInTunnel ? m_TunnelSpeed : m_Step;
		const auto& pos = diji::Collision::GetInstance().GetCollidingWithIntersectionRectf(shape, speed);
		if (pos != glm::vec2{ 0, 0 })
		{
			ghost->GetTransform()->SetPosition(pos.x - shape.width * 0.5f, pos.y - shape.height * 0.5f);
			if (not ghost->GetIsInMenu())
				CalculateDirection(ghost, target);
			m_TempLock = true;
		}
	}
	else if (m_TempLock)
	{
		m_LockedFrames++;
		if (m_LockedFrames >= 5)
		{
			m_TempLock = false;
			m_LockedFrames = 0;
		}
	}
}

void pacman::GhostState::CalculateDirection(const GhostAI* ghost, const glm::vec2& target)
{
	const auto& transform = ghost->GetTransform();
	const auto& collider = ghost->GetCollider();
	const auto& shape = collider->GetCollisionBox();
	const auto& updatedPos = transform->GetPosition();
	std::map<diji::Movement, bool> possibleDirections =
	{
		{ diji::Movement::Up, true },
		{ diji::Movement::Left, true },
		{ diji::Movement::Down, true },
		{ diji::Movement::Right, true }
	};

	//remove the opposite direction from which you came
	possibleDirections[static_cast<diji::Movement>((static_cast<int>(transform->GetMovement()) + 2) % 4)] = false;

	//remove directions that are blocked
	const glm::vec2 center(updatedPos.x + shape.width * 0.5f, updatedPos.y + shape.height * 0.5f);

	for (auto& [direction, canMove] : possibleDirections)
	{
		if (!canMove)
			continue;

		if (direction == diji::Movement::Up)
		{
			if (std::find(m_BlockedIntersections.begin(), m_BlockedIntersections.end(), center) != m_BlockedIntersections.end())
			{
				canMove = false;
				continue;
			}
		}

		const glm::vec2 nextTilePosition = center + GetTargetTranslation(direction);


		if (diji::Collision::GetInstance().IsCollidingWithWorld(center, nextTilePosition))
			canMove = false;
	}

	//check the remaining directions to determine the closest one
	//I could use a bool passed in the arguments to change between Frightened and other States
	//but I still need to pass a target and no other state has a target of { -1, -1 }
	diji::Movement bestDirection = diji::Movement::Idle;
	bool playerDirectionIsValid = false;
	if (ghost->IsPLayerControlled() && (dynamic_cast<Chase*>(this) or dynamic_cast<Frightened*>(this)))
	{
		const auto& playerChoice = ghost->GetNextMovement();
		auto it = possibleDirections.find(playerChoice);

		if (it != possibleDirections.end() && it->second)
		{
			bestDirection = playerChoice;
			playerDirectionIsValid = true;
		}
	}

	if (not playerDirectionIsValid)
	{
		if (target == glm::vec2{ -1 , -1 }) //Frightened
			bestDirection = ChooseRandomDirection(possibleDirections);
		else
		{
			float smallestDistance = std::numeric_limits<float>::max();

			for (const auto& [direction, canMove] : possibleDirections)
			{
				if (!canMove) continue;

				const glm::vec2 newPosition = center + GetTargetTranslation(direction);
				const float distance = glm::distance(newPosition, target);

				if (distance < smallestDistance)
				{
					smallestDistance = distance;
					bestDirection = direction;
				}
				else if (distance == smallestDistance)
				{
					if (static_cast<int>(direction) < static_cast<int>(bestDirection))
						continue;

					bestDirection = direction;
				}
			}
		}
	}

	transform->SetMovement(bestDirection);
	if (m_DisplayDirection)
		ghost->GetTexture()->SetStartingFrame(static_cast<int>(bestDirection) * 2);
}

glm::vec2 pacman::GhostState::GetTargetTranslation(diji::Movement movement) const
{
	glm::vec2 translation{ 0, 0 };
	switch (movement)
	{
	case diji::Movement::Right:
		translation.x = 16;
		break;
	case diji::Movement::Down:
		translation.y = 16;
		break;
	case diji::Movement::Left:
		translation.x = -16;
		break;
	case diji::Movement::Up:
		translation.y = -16;
		break;
	}

	return translation;
}

void pacman::GhostState::GoToTarget(const GhostAI* ghost, const glm::vec2& target)
{
	const auto& transform = ghost->GetTransform();
	const auto& collider = ghost->GetCollider();

	SeekTarget(ghost, target);

	const float movement = m_IsInTunnel ? m_TunnelSpeed : m_Step;
	const auto position = transform->GetPosition() + transform->GetMovementVector(movement);

	auto shape = collider->GetCollisionBox();
	shape.left = position.x;
	shape.bottom = position.y;

	//if (not diji::Collision::GetInstance().IsCollidingWithWorld(shape)) //check for collision just in case ghost goes over intersection trigger
		transform->SetPosition(position);
}

diji::Movement pacman::GhostState::ChooseRandomDirection(const std::map<diji::Movement, bool>& possibleDirections) const
{
	std::vector<diji::Movement> availableDirections;
	for (const auto& [direction, canMove] : possibleDirections)
		if (canMove)
			availableDirections.push_back(direction);

	std::srand(static_cast<unsigned int>(std::time(0)));
	const int randomIndex = std::rand() % availableDirections.size();

	return availableDirections[randomIndex];
}
#pragma endregion
#pragma region Eaten
void pacman::Eaten::OnEnter(const GhostAI* ghost)
{
	const auto& texture = ghost->GetTexture();
	texture->SetTexture("GhostEaten.png");
	std::swap(m_Step, m_EatenSpeed);
	
	if (ghost->GetIsInMenu())
	{
		texture->SetNrOfFrames(1);
		texture->SetCurrentFrame(9);
		texture->PauseAnimation();
		return;
	}

	diji::ServiceLocator::GetSoundSystem().AddSoundRequest("retreating.wav", true, -1);
}

void pacman::Eaten::OnExit(const GhostAI*)
{
	std::swap(m_Step, m_EatenSpeed);
}

std::unique_ptr<pacman::GhostState> pacman::Eaten::Execute(const GhostAI* ghost)
{
	if (ghost->GetIsInMenu())
		return nullptr;

	const auto& transform = ghost->GetTransform();
	const auto& collider = ghost->GetCollider();
	SeekTarget(ghost, m_SpawnPoint);

	const auto position = transform->GetPosition() + transform->GetMovementVector(m_Step);
	auto shape = collider->GetCollisionBox();
	shape.left = position.x;
	shape.bottom = position.y;

	if (not diji::Collision::GetInstance().IsCollidingWithWorld(shape))
		transform->SetPosition(position);
	else
		return nullptr;

	const glm::vec2 center(shape.left + shape.width * 0.5f, shape.bottom + shape.height * 0.5f);

	if (glm::distance(center, m_SpawnPoint) <= 2.f)
		return std::make_unique<Respawn>();

	return nullptr;
}
#pragma endregion
#pragma region Respawn
void pacman::Respawn::OnEnter(const GhostAI* ghost) 
{ 
	m_PersonnalSpawn = ghost->GetSpawnPoint();
	std::swap(m_Step, m_RespawnSpeed);
};

void pacman::Respawn::OnExit(const GhostAI* ghost)
{
	ghost->SetGhostTexture();
	ghost->GetTexture()->SetStartingFrame(static_cast<int>(diji::Movement::Up) * 2);
	std::swap(m_Step, m_RespawnSpeed);

	//check if other ghosts are still eaten
	bool otherGhostEaten = false;
	for (const auto& ai : ghost->GetGhostsAI())
	{
		if (ai != ghost and dynamic_cast<Eaten*>(ai->GetCurrentState()))
			otherGhostEaten = true;
	}
	
	auto& ss = diji::ServiceLocator::GetSoundSystem();

	if (ghost->IsLastGhostEaten())
	{
		ss.AddSoundRequest("siren_1.wav", true, -1);
		ghost->SetIsLastGhostEaten(false);
	}
	else if (not otherGhostEaten)
	{
		if (ghost->GetPowerUpTimer() == 0.f)
			ss.AddSoundRequest("siren_1.wav", true, -1);
		else
			ss.AddSoundRequest("power_pellet.wav", true, -1);
	}
}

std::unique_ptr<pacman::GhostState> pacman::Respawn::Execute(const GhostAI* ghost)
{
	const auto& transform = ghost->GetTransform();

	glm::vec3 currentPosition = transform->GetPosition();
	diji::Movement currentMovement = diji::Movement::Idle;

	if (currentPosition.y < m_PersonnalSpawn.y)
	{
		currentPosition.y += m_Step;
		currentMovement = diji::Movement::Down;
	}
	else
	{
		if (currentPosition.x < m_PersonnalSpawn.x)
		{
			currentPosition.x += m_Step;
			currentMovement = diji::Movement::Right;
		}
		else if (currentPosition.x > m_PersonnalSpawn.x)
		{
			currentPosition.x -= m_Step;
			currentMovement = diji::Movement::Left;
		}
	}
	
	if (glm::distance(glm::vec2{ currentPosition.x, currentPosition .y }, m_PersonnalSpawn) <= 3.f)
		transform->SetPosition(m_PersonnalSpawn);
	else
		transform->SetPosition(currentPosition);

	ghost->GetTexture()->SetStartingFrame(static_cast<int>(currentMovement) * 2);

	if (currentPosition.x == m_PersonnalSpawn.x and currentPosition.y == m_PersonnalSpawn.y)
	{
		return std::make_unique<ExitMaze>();
	}

	return nullptr;
}
#pragma endregion
#pragma region Exit Maze
void pacman::ExitMaze::OnEnter(const GhostAI* ghost)
{
	std::swap(m_Step, m_RespawnSpeed);
	ghost->GetTexture()->SetCurrentFrame(0);
	ghost->ClearFrightened();
}
void pacman::ExitMaze::OnExit(const GhostAI* ghost)
{
	ghost->GetTransform()->SetMovement(diji::Movement::Left);
	ghost->GetTexture()->SetStartingFrame(static_cast<int>(diji::Movement::Left) * 2);
	std::swap(m_Step, m_RespawnSpeed);
}

std::unique_ptr<pacman::GhostState> pacman::ExitMaze::Execute(const GhostAI* ghost)
{
	const auto& transform = ghost->GetTransform();

	glm::vec3 currentPosition = transform->GetPosition();
	diji::Movement currentMovement = diji::Movement::Idle;

	if (currentPosition.x < m_OutsidePosition.x)
	{
		currentPosition.x += m_Step;
		currentMovement = diji::Movement::Right;
	}
	else if (currentPosition.x > m_OutsidePosition.x)
	{
		currentPosition.x -= m_Step;
		currentMovement = diji::Movement::Left;
	}
	else
	{
		if (currentPosition.y > m_OutsidePosition.y)
		{
			currentPosition.y -= m_Step;
			currentMovement = diji::Movement::Up;
		}
		else if (glm::distance(glm::vec2{ currentPosition.x, currentPosition.y }, m_OutsidePosition) <= 2.f)
		{
			transform->SetPosition(m_OutsidePosition);
			return (ghost->GetIsInChaseState() or ghost->IsPLayerControlled()) ? ghost->GetChaseState() : std::make_unique<Scatter>();
		}
	}

	transform->SetPosition(currentPosition);
	ghost->GetTexture()->SetStartingFrame(static_cast<int>(currentMovement) * 2);

	return nullptr;
}
#pragma endregion
#pragma region Scatter
void pacman::Scatter::OnEnter(const GhostAI* ghost)
{
	m_Target = ghost->GetScatterTarget();
	ghost->TurnAround();
	ghost->GetTexture()->SetStartingFrame(static_cast<int>(ghost->GetTransform()->GetMovement()) * 2);
}

std::unique_ptr<pacman::GhostState> pacman::Scatter::Execute(const GhostAI* ghost)
{
	GoToTarget(ghost, m_Target);

	if (ghost->IsPLayerControlled())
		return ghost->GetChaseState();

	if (ghost->GetIsInChaseState())
		return ghost->GetChaseState();

	if (ghost->IsFrightened())
		return std::make_unique<Frightened>();

	return nullptr;
}
#pragma endregion
#pragma region Frightened
void pacman::Frightened::OnEnter(const GhostAI* ghost)
{
	ghost->TurnAround();
	const auto& texture = ghost->GetTexture();
	texture->SetTexture("Frightened.png");
	texture->SetNrOfFrames(2);
	texture->SetStartingFrame(0);
	m_DisplayDirection = false;
	m_IsUpdated = false;
	
	if (ghost->GetIsInMenu())
		m_FrightSpeed = 1.f;
	
	std::swap(m_Step, m_FrightSpeed);
}

void pacman::Frightened::OnExit(const GhostAI* ghost)
{
	ghost->SetGhostTexture();
	const auto& texture = ghost->GetTexture();
	texture->SetNrOfFrames(2);
	texture->SetStartingFrame(static_cast<int>(ghost->GetTransform()->GetMovement()) * 2);
	texture->DisableFlickerAnimation();
	m_DisplayDirection = true;
	m_IsUpdated = false;
	std::swap(m_Step, m_FrightSpeed);
}

std::unique_ptr<pacman::GhostState> pacman::Frightened::Execute(const GhostAI* ghost)
{
	const auto& transform = ghost->GetTransform();
	const auto& collider = ghost->GetCollider();

	SeekTarget(ghost, glm::vec2{ -1, -1 });

	const auto position = transform->GetPosition() + transform->GetMovementVector(m_Step);

	auto shape = collider->GetCollisionBox();
	shape.left = position.x;
	shape.bottom = position.y;

	//if (not diji::Collision::GetInstance().IsCollidingWithWorld(shape)) //check for collision just in case ghost goes over intersection trigger
		transform->SetPosition(position);

	if (not m_IsUpdated and ghost->IsPowerAlmostOver())
	{
		ghost->GetTexture()->SetNrOfFrames(4);
		ghost->GetTexture()->EnableFlickerAnimation();
		m_IsUpdated = true;
	}

	if (not ghost->IsFrightened())
		return (ghost->GetIsInChaseState() or ghost->IsPLayerControlled()) ? ghost->GetChaseState() : std::make_unique<Scatter>();

	return nullptr;
}

#pragma endregion
#pragma region Chase
void pacman::Chase::OnEnter(const GhostAI* ghost)
{
	ghost->TurnAround();
	ghost->GetTexture()->SetStartingFrame(static_cast<int>(ghost->GetTransform()->GetMovement()) * 2);
	
	if (ghost->GetIsInMenu())
		m_Step = 2;
}

std::unique_ptr<pacman::GhostState> pacman::RedChase::Execute(const GhostAI* ghost)
{
	const auto& player = ghost->GetPlayerCollider();

	const auto& playerPos = player->GetCollisionBox();
	const glm::vec2 target(playerPos.left + playerPos.width * 0.5f, playerPos.bottom + playerPos.height * 0.5f);

	GoToTarget(ghost, target);

	if (ghost->IsFrightened())
		return std::make_unique<Frightened>();

	//could add the additional check for pellets remaining here
	if (not ghost->GetIsInMenu())
		if (not ghost->IsPLayerControlled() and not ghost->GetIsInChaseState())
			return std::make_unique<Scatter>();
	
	return nullptr;
}

std::unique_ptr<pacman::GhostState> pacman::PinkyChase::Execute(const GhostAI* ghost)
{
	const auto& player = ghost->GetPlayerCollider();
	const auto& transform = ghost->GetPlayerTransform();

	const auto& playerPos = player->GetCollisionBox();
	const glm::vec2 center(playerPos.left + playerPos.width * 0.5f, playerPos.bottom + playerPos.height * 0.5f);
	glm::vec2 target = center + transform->Get2DMovementVector(m_TargetDistance);

	const auto& playerDirection = transform->GetLookingDirection();
	if (playerDirection == diji::Movement::Up or (playerDirection == diji::Movement::Idle and transform->GetLastMovement() == diji::Movement::Up))
		target.x -= m_TargetDistance;

	GoToTarget(ghost, target);

	if (not ghost->GetIsInChaseState())
		return std::make_unique<Scatter>();

	if (ghost->IsFrightened())
		return std::make_unique<Frightened>();

	return nullptr;
}

std::unique_ptr<pacman::GhostState> pacman::InkyChase::Execute(const GhostAI* ghost)
{
	constexpr int GHOST_WIDTH = 15;
	constexpr int GHOST_HEIGHT = 15;

	const auto& player = ghost->GetPlayerCollider();
	const auto& transform = ghost->GetPlayerTransform();

	const auto& playerPos = player->GetCollisionBox();
	const glm::vec2 center(playerPos.left + playerPos.width * 0.5f, playerPos.bottom + playerPos.height * 0.5f);
	glm::vec2 target = center + transform->Get2DMovementVector(m_TargetDistance);

	const auto& playerDirection = transform->GetLookingDirection();
	if (playerDirection == diji::Movement::Up or (playerDirection == diji::Movement::Idle and transform->GetLastMovement() == diji::Movement::Up))
		target.x -= m_TargetDistance;

	const glm::vec3& blinkyPos = ghost->GetSecondGhostTransform()->GetPosition();
	const glm::vec2& blinkyCenter = glm::vec2{ blinkyPos.x + GHOST_WIDTH, blinkyPos.y + GHOST_HEIGHT };
	const glm::vec2 targetToBlinky = blinkyCenter - target;
	const glm::vec2 newTarget = target - targetToBlinky;

	GoToTarget(ghost, newTarget);

	if (not ghost->GetIsInChaseState())
		return std::make_unique<Scatter>();

	if (ghost->IsFrightened())
		return std::make_unique<Frightened>();

	return nullptr;
}

std::unique_ptr<pacman::GhostState> pacman::ClydeChase::Execute(const GhostAI* ghost)
{
	const auto& player = ghost->GetPlayerCollider();
	const auto& colliderRect = ghost->GetCollider()->GetCollisionBox();
	const auto& playerPos = player->GetCollisionBox();
	const glm::vec2 playerCenter(playerPos.left + playerPos.width * 0.5f, playerPos.bottom + playerPos.height * 0.5f);
	const glm::vec2 clydeCenter(colliderRect.left + colliderRect.width * 0.5f, colliderRect.bottom + colliderRect.height * 0.5f);

	glm::vec2 target = glm::distance(playerCenter, clydeCenter) >= 128.f ? playerCenter : ghost->GetScatterTarget();
	GoToTarget(ghost, target);

	if (not ghost->GetIsInChaseState())
		return std::make_unique<Scatter>();

	if (ghost->IsFrightened())
		return std::make_unique<Frightened>();

	return nullptr;
}
#pragma endregion
#pragma region Dying
void pacman::Dying::OnEnter(const GhostAI* ghost)
{
	if (!ghost->GetIsInMenu())
		diji::ServiceLocator::GetSoundSystem().AddSoundRequest("eat_ghost.wav", false, -1);

	const auto& texture = ghost->GetTexture();
	texture->SetTexture(std::to_string(m_Points) + ".png");
	texture->SetNrOfFrames(1);
	texture->PauseAnimation();
	texture->SetCurrentFrame(0);

	if (m_Points == 1600)
		texture->SetWidth(16);
}

void pacman::Dying::OnExit(const GhostAI* ghost)
{
	const auto& texture = ghost->GetTexture();

	texture->SetNrOfFrames(2);
	texture->ResumeAnimation();

	if (m_Points == 1600)
	{
		texture->SetWidth(15);
		ghost->SetIsLastGhostEaten(true);
	}
}

std::unique_ptr<pacman::GhostState> pacman::Dying::Execute(const GhostAI* ghost)
{
	return ghost->IsUpdatePaused() ? nullptr : std::make_unique<Eaten>();
}
#pragma endregion
#pragma region Waiting
pacman::Waiting::Waiting(const int pellets)
	: m_PelletsNeeded{ pellets }
{
}

void pacman::Waiting::OnEnter(const GhostAI* ghost)
{
	ghost->GetTexture()->SetStartingFrame(static_cast<int>(ghost->GetTransform()->GetMovement()) * 2);
	std::swap(m_Step, m_WaitSpeed);

}

void pacman::Waiting::OnExit(const GhostAI*)
{
	std::swap(m_Step, m_WaitSpeed);
}

std::unique_ptr<pacman::GhostState> pacman::Waiting::Execute(const GhostAI* ghost)
{
	const auto& transform = ghost->GetTransform();
	const auto& texture = ghost->GetTexture();
	glm::vec3 currentPosition = transform->GetPosition();

	constexpr float upperBound = 300.0f - 16.0f;
	constexpr float lowerBound = 300.0f + 2.0f;

	if (m_IsGoingUp)
	{
		if (currentPosition.y > upperBound)
			currentPosition.y -= m_Step; 
		else
		{
			m_IsGoingUp = false; 
			texture->SetStartingFrame(static_cast<int>(diji::Movement::Down) * 2);
		}
	}
	else
	{
		if (currentPosition.y < lowerBound)
			currentPosition.y += m_Step;
		else
		{
			m_IsGoingUp = true;
			texture->SetStartingFrame(static_cast<int>(diji::Movement::Up) * 2);
		}
	}

	transform->SetPosition(currentPosition);
	return (ghost->GetPelletCount() >= m_PelletsNeeded) ? std::make_unique<pacman::ExitMaze>() : nullptr;
}
#pragma endregion