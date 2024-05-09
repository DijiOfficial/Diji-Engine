#pragma once
#include "GameActorCommands.h"
#include <map>

namespace diji
{
	class Transform;
	class HealthCounter;
	class ScoreCounter;
	class Collider;
	enum class PointType;

	class Move final : public GameActorCommands
	{
	public:
		Move(GameObject* actor, Movement movement);
		~Move() override = default;

		void Execute() override;

	private:
		const glm::vec2 m_Speed = { 160.f, 160.f }; //sets speed as universal, may change it to get it from the actor
		Movement m_Movement;
		Transform* m_TransformComponentPtr;
		Collider* m_CollisionComponentPtr;
	};

	class HitCommand final : public GameActorCommands
	{

	public:
		HitCommand(GameObject* actor);
		~HitCommand() override = default;

		void Execute() override;

	private:
		HealthCounter* m_HealthComponentPtr;
	};

	class ScoreCommand final : public GameActorCommands
	{

	public:
		ScoreCommand(GameObject* actorPtr, PointType point);
		~ScoreCommand() override = default;

		void Execute() override;

	private:
		ScoreCounter* m_ScoreComponentPtr;
		PointType m_PointType;
	};
}

