#include "GhostAI.h"
#include "Collider.h"
#include "Transform.h"
#include "Texture.h"
#include "AI.h"
#include "Observers.h"

#include "RedAI.h"

pacman::GhostAI::GhostAI(diji::GameObject* ownerPtr, diji::GameObject* player)
	: Component(ownerPtr)
	, m_PlayerColliderPtr{ player->GetComponent<diji::Collider>() }
{
	//todo: start function for everything this is old now
	m_TransformCompPtr = ownerPtr->GetComponent<diji::Transform>();
	m_ColliderCompPtr = ownerPtr->GetComponent<diji::Collider>();
	m_TextureCompPtr = ownerPtr->GetComponent<diji::Texture>();

	assert(m_TransformCompPtr and "AI Component needs to be initialized aftera Transform");
	assert(m_ColliderCompPtr and "AI Component needs to be initialized aftera Collider");
	assert(m_TextureCompPtr and "AI Component needs to be initialized aftera Texture");

	m_TransformCompPtr->SetMovement(diji::Movement::Up);
	m_PlayerAICompPtr = player->GetComponent<AI>();
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
#pragma region GhostState
void pacman::GhostState::SeekTarget(const GhostAI* ghost, const glm::vec2& target)
{
	const auto& collider = ghost->GetCollider();
	const auto& shape = collider->GetCollisionBox();

	if (diji::Collision::GetInstance().IsCollidingWithIntersection(shape) and not m_TempLock)
	{
		CalculateDirection(ghost, target);
		m_TempLock = true;
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
	const glm::vec2 center(shape.left + shape.width * 0.5f, shape.bottom + shape.height * 0.5f);

	for (auto& [direction, canMove] : possibleDirections)
	{
		if (!canMove) 
			continue;

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

	if (not diji::Collision::GetInstance().IsCollidingWithWorld(shape)) //check for collision just in case ghost goes over intersection trigger
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
	m_NextStateUPtr = std::make_unique<Respawn>();
	const auto& texture = ghost->GetTexture();
	texture->SetTexture("GhostEaten.png");
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

	if (center == m_SpawnPoint)
		return std::move(m_NextStateUPtr);

	return nullptr;
}
#pragma endregion
#pragma region Respawn
void pacman::Respawn::OnExit(const GhostAI* ghost)
{
	const auto& texture = ghost->GetTexture();
	//todo: have ghost hold a texture string
	if (const RedAI* redAI = dynamic_cast<const RedAI*>(ghost))
	{
		texture->SetTexture("RedGhost.png");
	}
	texture->SetStartingFrame(static_cast<int>(diji::Movement::Up) * 2);
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
	ghost->SetIsPoweredUpLock();
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
		else
			return ghost->GetIsInChaseState() ? ghost->GetChaseState() : std::make_unique<Scatter>();
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

	if (ghost->GetIsPoweredUp() and not ghost->GetIsPoweredUpLock())
		return std::make_unique<Frightened>();
	else if (not ghost->GetIsPoweredUp() and ghost->GetIsPoweredUpLock())
		ghost->SetIsPoweredUpLock();

	return nullptr;
}
#pragma endregion
#pragma region Frightened
void pacman::Frightened::OnEnter(const GhostAI* ghost)
{
	ghost->TurnAround();
	const auto& texture = ghost->GetTexture();
	texture->SetTexture("Frightened.png");
	texture->SetStartingFrame(0);
	m_DisplayDirection = false;
}

void pacman::Frightened::OnExit(const GhostAI* ghost)
{
	const auto& texture = ghost->GetTexture();
	texture->SetTexture("RedGhost.png");
	m_DisplayDirection = true;
	ghost->GetTexture()->SetStartingFrame(static_cast<int>(ghost->GetTransform()->GetMovement()) * 2);
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

	if (not diji::Collision::GetInstance().IsCollidingWithWorld(shape)) //check for collision just in case ghost goes over intersection trigger
		transform->SetPosition(position);

	const auto& test = diji::Collision::GetInstance().IsColliding(collider);
	for (const auto& colliders : test)
	{
		if (colliders == player)
		{
			return std::make_unique<Eaten>();
		}
	}

	if (not ghost->GetIsPoweredUp())
		return ghost->GetIsInChaseState() ? ghost->GetChaseState() : std::make_unique<Scatter>();

	return nullptr;
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

	if (ghost->GetIsPoweredUp() and not ghost->GetIsPoweredUpLock())
		return std::make_unique<Frightened>();
	else if (not ghost->GetIsPoweredUp() and ghost->GetIsPoweredUpLock())
		ghost->SetIsPoweredUpLock();

	return nullptr;
}
#pragma endregion