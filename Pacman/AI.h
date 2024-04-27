#pragma once
#include "Command.h"
#include "IObserver.h"

namespace diji {
	class Texture;
	class Transform;

	class AI final : public Component, public IObserver
	{
	public:
		AI(GameObject* ownerPtr);
		~AI() override = default;

		AI(const AI& other) = delete;
		AI(AI&& other) = delete;
		AI& operator=(const AI& other) = delete;
		AI& operator=(AI&& other) = delete;

		void Update() override;
		void OnNotify(MessageTypes message, Subject* subject) override;

	private:
		Texture* m_TextureCompPtr;
		Transform* m_TransformCompPtr;
		Movement m_PreviousMovement = Movement::Idle;
	};
}

