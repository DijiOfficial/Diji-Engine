#pragma once
#include "Command.h"

namespace diji
{
	class GameObject;

	class GameActorCommands : public Command
	{
	public:
		explicit GameActorCommands(GameObject* actor);
		~GameActorCommands() override = default;

	protected:
		GameObject* GetGameActor() const { return m_ActorPtr; }

	private:
		GameObject* m_ActorPtr;
	};
}


