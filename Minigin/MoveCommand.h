#pragma once
#include "Commands.h"
#include <map>

namespace diji 
{
	class Transform;

	class MoveCommand final : public Commands
	{

	public:
		MoveCommand(GameObject* actorPtr);
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
}

