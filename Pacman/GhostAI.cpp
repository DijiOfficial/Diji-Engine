#include "GhostAI.h"
#include "Collider.h"
#include "Transform.h"
#include "Texture.h"

#include "RedAI.h"
diji::GhostAI::GhostAI(GameObject* ownerPtr, GameObject* player)
	: Component(ownerPtr)
	, m_PlayerColliderPtr{ player->GetComponent<Collider>() }
{
	m_TransformCompPtr = ownerPtr->GetComponent<Transform>();
	m_ColliderCompPtr = ownerPtr->GetComponent<Collider>();
	m_TextureCompPtr = ownerPtr->GetComponent<Texture>();

	assert(m_TransformCompPtr and "AI Component needs to be initialized aftera Transform");
	assert(m_ColliderCompPtr and "AI Component needs to be initialized aftera Collider");
	assert(m_TextureCompPtr and "AI Component needs to be initialized aftera Texture");

	m_TransformCompPtr->SetMovement(Movement::Up);
}

void diji::GhostAI::FixedUpdate()
{
	auto state = m_CurrentStateUPtr->Execute(this);

	if (state)
	{
		m_CurrentStateUPtr->OnExit(this);
		m_CurrentStateUPtr = std::move(state);
		m_CurrentStateUPtr->OnEnter(this);
	}
}

void diji::GhostAI::TurnAround() const
{
	m_TransformCompPtr->SetMovement(static_cast<diji::Movement>((static_cast<int>(m_TransformCompPtr->GetMovement()) + 2) % 4));
}
#pragma region GhostState
void diji::GhostState::SeekTarget(const GhostAI* ghost, const glm::vec2& target)
{
	const auto& collider = ghost->GetCollider();
	const auto& shape = collider->GetCollisionBox();

	if (Collision::GetInstance().IsCollidingWithIntersection(shape) and not m_TempLock)
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

void diji::GhostState::CalculateDirection(const GhostAI* ghost, const glm::vec2& target)
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


		if (Collision::GetInstance().IsCollidingWithWorld(center, nextTilePosition))
			canMove = false;
	}

	//check the remaining directions to determine the closest one
	//I could use a bool passed in the arguments to change between Frightened and other States
	//but I still need to pass a target and no other state has a target of { 0, 0 }
	Movement bestDirection = Movement::Idle;
	
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
	ghost->GetTexture()->SetStartingFrame(static_cast<int>(bestDirection) * 2);
}

glm::vec2 diji::GhostState::GetTargetTranslation(Movement movement) const
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

void diji::GhostState::GoToTarget(const GhostAI* ghost, const glm::vec2& target)
{
	const auto& transform = ghost->GetTransform();
	const auto& collider = ghost->GetCollider();

	SeekTarget(ghost, target);

	const auto position = transform->GetPosition() + transform->GetMovementVector(m_Step);

	auto shape = collider->GetCollisionBox();
	shape.left = position.x;
	shape.bottom = position.y;

	if (not Collision::GetInstance().IsCollidingWithWorld(shape)) //check for collision just in case ghost goes over intersection trigger
		transform->SetPosition(position);
}

diji::Movement diji::GhostState::ChooseRandomDirection(const std::map<diji::Movement, bool>& possibleDirections) const
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
void diji::Eaten::OnEnter(const GhostAI* ghost)
{
	m_NextStateUPtr = std::make_unique<Respawn>();
	const auto& texture = ghost->GetTexture();
	texture->SetTexture("GhostEaten.png");
}

std::unique_ptr<diji::GhostState> diji::Eaten::Execute(const GhostAI* ghost)
{
	const auto& transform = ghost->GetTransform();
	const auto& collider = ghost->GetCollider();
	SeekTarget(ghost, m_SpawnPoint);

	const auto position = transform->GetPosition() + transform->GetMovementVector(m_Step);
	auto shape = collider->GetCollisionBox();
	shape.left = position.x;
	shape.bottom = position.y;

	if (not Collision::GetInstance().IsCollidingWithWorld(shape))
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
void diji::Respawn::OnExit(const GhostAI* ghost)
{
	const auto& texture = ghost->GetTexture();
	//todo: have ghost hold a texture string
	if (const RedAI* redAI = dynamic_cast<const RedAI*>(ghost))
	{
		texture->SetTexture("RedGhost.png");
	}
	texture->SetStartingFrame(static_cast<int>(diji::Movement::Up) * 2);
}

std::unique_ptr<diji::GhostState> diji::Respawn::Execute(const GhostAI* ghost)
{
	const auto& transform = ghost->GetTransform();

	glm::vec3 currentPosition = transform->GetPosition();
	Movement currentMovement = Movement::Idle;

	if (currentPosition.y < m_PersonnalSpawn.y)
	{
		currentPosition.y += m_Step;
		currentMovement = Movement::Down;
	}
	else
	{
		if (currentPosition.x < m_PersonnalSpawn.x)
		{
			currentPosition.x += m_Step;
			currentMovement = Movement::Right;
		}
		else if (currentPosition.x > m_PersonnalSpawn.x)
		{
			currentPosition.x -= m_Step;
			currentMovement = Movement::Left;
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
void diji::ExitMaze::OnExit(const GhostAI* ghost)
{
	ghost->GetTransform()->SetMovement(Movement::Left);
	ghost->GetTexture()->SetStartingFrame(static_cast<int>(Movement::Left) * 2);
}

std::unique_ptr<diji::GhostState> diji::ExitMaze::Execute(const GhostAI* ghost)
{
	const auto& transform = ghost->GetTransform();

	glm::vec3 currentPosition = transform->GetPosition();
	Movement currentMovement = Movement::Idle;

	if (currentPosition.x < m_OutsidePosition.x)
	{
		currentPosition.x += m_Step;
		currentMovement = Movement::Right;
	}
	else if (currentPosition.x > m_OutsidePosition.x)
	{
		currentPosition.x -= m_Step;
		currentMovement = Movement::Left;
	}
	else
	{
		if (currentPosition.y > m_OutsidePosition.y)
		{
			currentPosition.y -= m_Step;
			currentMovement = Movement::Up;
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
void diji::Scatter::OnEnter(const GhostAI* ghost)
{
	m_Target = ghost->GetScatterTarget();
	ghost->TurnAround();
}

std::unique_ptr<diji::GhostState> diji::Scatter::Execute(const GhostAI* ghost)
{
	GoToTarget(ghost, m_Target);

	if (ghost->GetIsInChaseState())
		return std::make_unique<RedChase>();

	return nullptr;
}
#pragma endregion
#pragma region Frightened
void diji::Frightened::OnEnter(const GhostAI* ghost)
{
	ghost->TurnAround();
	const auto& texture = ghost->GetTexture();
	texture->SetTexture("Frightened.png");
	texture->SetStartingFrame(0);
}

std::unique_ptr<diji::GhostState> diji::Frightened::Execute(const GhostAI* ghost)
{
	const auto& transform = ghost->GetTransform();
	const auto& collider = ghost->GetCollider();
	const auto& player = ghost->GetPlayerCollider();

	SeekTarget(ghost, glm::vec2{ 0, 0 });

	const auto position = transform->GetPosition() + transform->GetMovementVector(m_Step);

	auto shape = collider->GetCollisionBox();
	shape.left = position.x;
	shape.bottom = position.y;

	if (not Collision::GetInstance().IsCollidingWithWorld(shape)) //check for collision just in case ghost goes over intersection trigger
		transform->SetPosition(position);

	const auto& test = Collision::GetInstance().IsColliding(collider);
	for (const auto& colliders : test)
	{
		if (colliders == player)
		{
			return std::make_unique<Eaten>();
		}
	}

	return nullptr;
}
#pragma endregion
#pragma region Chase
void diji::RedChase::OnEnter(const GhostAI* ghost)
{
	ghost->TurnAround();
}

std::unique_ptr<diji::GhostState> diji::RedChase::Execute(const GhostAI* ghost)
{
	const auto& player = ghost->GetPlayerCollider();

	const auto& playerPos = player->GetCollisionBox();
	const glm::vec2 target(playerPos.left + playerPos.width * 0.5f, playerPos.bottom + playerPos.height * 0.5f);

	GoToTarget(ghost, target);

	//could add the additional check for pellets remaining here
	if (not ghost->GetIsInChaseState())
		return std::make_unique<Scatter>();

	return nullptr;
}
#pragma endregion