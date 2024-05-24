#include "GhostStates.h"
#include "GhostAI.h"
#include "Collider.h"
#include "Collision.h"
#include "Command.h"
#include "Transform.h"
#include "Texture.h"

#pragma region GhostState
void pacman::GhostState::SeekTarget(const GhostAI* ghost, const glm::vec2& target)
{
	const auto& collider = ghost->GetCollider();
	auto shape = collider->GetCollisionBox();

	shape.left = std::round(shape.left);
	shape.bottom = std::round(shape.bottom);
	
	if (not m_TempLock)
	{
		const auto& pos = diji::Collision::GetInstance().GetCollidingWithIntersectionRectf(shape);
		if (pos != glm::vec2{ 0, 0 })
		{
			ghost->GetTransform()->SetPosition(pos.x - shape.width * 0.5f, pos.y - shape.height * 0.5f);
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
	//but I still need to pass a target and no other state has a target of { 0, 0 }
	diji::Movement bestDirection = diji::Movement::Idle;

	if (target == glm::vec2{ 0 , 0 }) //Frightened
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

	const auto position = transform->GetPosition() + transform->GetMovementVector(m_Step);

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
}

void pacman::Eaten::OnExit(const GhostAI*)
{
	std::swap(m_Step, m_EatenSpeed);
}

std::unique_ptr<pacman::GhostState> pacman::Eaten::Execute(const GhostAI* ghost)
{
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
	
	if (glm::distance(glm::vec2{ currentPosition.x, currentPosition .y }, m_PersonnalSpawn) <= 2.f)
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
void pacman::ExitMaze::OnExit(const GhostAI* ghost)
{
	ghost->GetTransform()->SetMovement(diji::Movement::Left);
	ghost->GetTexture()->SetStartingFrame(static_cast<int>(diji::Movement::Left) * 2);
	ghost->ClearFrightened();
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
			return ghost->GetIsInChaseState() ? ghost->GetChaseState() : std::make_unique<Scatter>();
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

	if (ghost->GetIsInChaseState())
		return std::make_unique<RedChase>();

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
	//todo: check why ghost is gone for 1 frame
	texture->SetTexture("Frightened.png");
	texture->SetNrOfFrames(2);
	texture->SetStartingFrame(0);
	m_DisplayDirection = false;
	m_IsUpdated = false;
	std::swap(m_Step, m_FrightSpeed);
}

void pacman::Frightened::OnExit(const GhostAI* ghost)
{
	const auto& texture = ghost->GetTexture();
	texture->SetTexture("RedGhost.png");
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
	const auto& player = ghost->GetPlayerCollider();

	SeekTarget(ghost, glm::vec2{ 0, 0 });

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

	const auto& test = diji::Collision::GetInstance().IsColliding(collider);
	for (const auto& colliders : test)
	{
		if (colliders == player)
		{
			return std::make_unique<Eaten>();
		}
	}

	if (not ghost->IsFrightened())
		return ghost->GetIsInChaseState() ? ghost->GetChaseState() : std::make_unique<Scatter>();

	return nullptr;
}

void pacman::Frightened::SwitchSpeed()
{
}
#pragma endregion
#pragma region Chase
void pacman::RedChase::OnEnter(const GhostAI* ghost)
{
	ghost->TurnAround();
	ghost->GetTexture()->SetStartingFrame(static_cast<int>(ghost->GetTransform()->GetMovement()) * 2);
}

std::unique_ptr<pacman::GhostState> pacman::RedChase::Execute(const GhostAI* ghost)
{
	const auto& player = ghost->GetPlayerCollider();

	const auto& playerPos = player->GetCollisionBox();
	const glm::vec2 target(playerPos.left + playerPos.width * 0.5f, playerPos.bottom + playerPos.height * 0.5f);

	GoToTarget(ghost, target);

	//could add the additional check for pellets remaining here
	if (not ghost->GetIsInChaseState())
		return std::make_unique<Scatter>();

	if (ghost->IsFrightened())
		return std::make_unique<Frightened>();
	
	return nullptr;
}
#pragma endregion