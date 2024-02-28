#pragma once
#include "GameObject.h"

namespace diji
{
	class Text;
	class GameObject;

	class FPSCounter final : public Component
	{
	public:
		FPSCounter(GameObject* ownerPtr) : Component(ownerPtr), m_FrameCount{ 0 }, m_Fps{ 0 }, m_ElapsedTime{ 0 } { m_TextComponentPtr = ownerPtr->GetComponent<Text>(); };
		~FPSCounter() override = default;

		FPSCounter(const FPSCounter& other) = delete;
		FPSCounter(FPSCounter&& other) = delete;
		FPSCounter& operator=(const FPSCounter& other) = delete;
		FPSCounter& operator=(FPSCounter&& other) = delete;

		void Update() override;
		float GetFPS() const { return m_Fps; };

	private:
		const float REFRESH_RATE = 0.1f;
		int m_FrameCount;
		float m_Fps;
		float m_ElapsedTime;
		Text* m_TextComponentPtr;
	};
}

