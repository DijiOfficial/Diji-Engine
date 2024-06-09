#pragma once
#include "Component.h"
namespace diji
{
	class Render;
}
namespace pacman 
{
	class BlinkingText final : public diji::Component
	{
	public:
		BlinkingText(diji::GameObject* ownerPtr) : Component(ownerPtr) {};
		~BlinkingText() noexcept override = default;

		void Init() override;
		void Update() override;
		void FixedUpdate() override {};
		
	private:
		diji::Render* m_RenderCompPtr = nullptr;
		float m_BlinkTimer = 0.0f;
		bool m_IsVisible = true;
	};
}

