#pragma once
#include "Command.h"

namespace diji
{
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
}


