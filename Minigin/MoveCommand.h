#pragma once
#include "Commands.h"
#include <map>

#include "ScoreCounter.h"
namespace diji 
{
	class Transform;
	class HealthCounter;
	class ScoreCounter;
	enum class PointType;

	class MoveCommand final : public Commands
	{

	public:
		MoveCommand(const GameObject* actorPtr);
		~MoveCommand() override = default;

		MoveCommand(const MoveCommand& other) = delete;
		MoveCommand(MoveCommand&& other) = delete;
		MoveCommand& operator=(const MoveCommand& other) = delete;
		MoveCommand& operator=(MoveCommand&& other) = delete;

		void Execute() override;

		void KeyPressed(const Movement& movement) { m_Movement[movement] = true; }
		void KeyReleased(const Movement& movement) { m_Movement[movement] = false; }

	private:
		const glm::vec2 m_Speed = { 100.f, 100.f };

		std::map<Movement, bool> m_Movement = { {Movement::Up, false}, {Movement::Down, false},{Movement::Left, false},{Movement::Right, false} };
		Transform* m_TransformComponentPtr;
	};

	class HitCommand final : public Commands
	{

	public:
		//why is this overloaded?
		HitCommand(const GameObject* actorPtr);
		~HitCommand() override = default;

		HitCommand(const HitCommand& other) = delete;
		HitCommand(HitCommand&& other) = delete;
		HitCommand& operator=(const HitCommand& other) = delete;
		HitCommand& operator=(HitCommand&& other) = delete;

		void Execute() override;

		void KeyReleased() { m_IsHit = true; };
	
	private:
		HealthCounter* m_HealthComponentPtr;
		bool m_IsHit = false;
	};

	class ScoreCommand final
	{

	public:
		ScoreCommand(const GameObject* actorPtr);
		~ScoreCommand() = default;

		ScoreCommand(const ScoreCommand& other) = delete;
		ScoreCommand(ScoreCommand&& other) = delete;
		ScoreCommand& operator=(const ScoreCommand& other) = delete;
		ScoreCommand& operator=(ScoreCommand&& other) = delete;

		//void Execute() override;

		void KeyReleased(PointType pointType) { m_ScoreComponentPtr->HitEnemy(pointType); };

	private:
		ScoreCounter* m_ScoreComponentPtr;
	};
}

