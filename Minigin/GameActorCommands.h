#pragma once
#include "Command.h"
#include <map>

namespace diji
{
	class Transform;
	class HealthCounter;
	class ScoreCounter;
	enum class PointType;

	class GameActorCommands : public Command
	{
	public:
		explicit GameActorCommands(GameObject* actor) : m_ActorPtr(actor) {};
		~GameActorCommands() override = default;

	protected:
		GameObject* GetGameActor() const { return m_ActorPtr; }

	private:
		GameObject* m_ActorPtr;
	};

	class Move final: public GameActorCommands
	{
	public:
		Move(GameObject* actor, Movement movement);
		~Move() override = default;
		
		void Execute() override;
	
	private:
		const glm::vec2 m_Speed = { 300.f, 300.f }; //sets speed as universal, may change it to get it from the actor
		Movement m_Movement;
		Transform* m_TransformComponentPtr;
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


