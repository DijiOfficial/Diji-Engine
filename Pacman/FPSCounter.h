#pragma once
#include "Component.h"

namespace diji
{
	class Text;
	class FPSCounter final : public Component
	{
	public:
		FPSCounter(GameObject* ownerPtr);
		~FPSCounter() override = default;

		FPSCounter(const FPSCounter& other) = delete;
		FPSCounter(FPSCounter&& other) = delete;
		FPSCounter& operator=(const FPSCounter& other) = delete;
		FPSCounter& operator=(FPSCounter&& other) = delete;

		void Init() override;
		void Update() override;
		void FixedUpdate() override {};
		float GetFPS() const { return m_Fps; };

	private:
		Text* m_TextComponentPtr;
		const float REFRESH_RATE = 0.1f;
		int m_FrameCount;
		float m_Fps;
		float m_ElapsedTime;
	};
}

