#pragma once
#include "Command.h"
#include <map>

namespace diji
{
	class Transform;
	class HealthCounter;

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

	//class ScoreCommand final
	//{

	//public:
	//	ScoreCommand(const GameObject* actorPtr);
	//	~ScoreCommand() = default;

	//	ScoreCommand(const ScoreCommand& other) = delete;
	//	ScoreCommand(ScoreCommand&& other) = delete;
	//	ScoreCommand& operator=(const ScoreCommand& other) = delete;
	//	ScoreCommand& operator=(ScoreCommand&& other) = delete;

	//	//void Execute() override;

	//	void KeyReleased(PointType pointType) { m_ScoreComponentPtr->HitEnemy(pointType); };

	//private:
	//	ScoreCounter* m_ScoreComponentPtr;
	//};
}


