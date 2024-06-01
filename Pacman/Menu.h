#pragma once
#include "Component.h"
#include "Collision.h"

namespace pacman
{
	class Menu final : public diji::Component
	{
	public:
		Menu(diji::GameObject* ownerPtr) : Component(ownerPtr) {};
		~Menu() noexcept override = default;

		void Init() override {};
		void Update() override;
		void FixedUpdate() override {};

	private:
		diji::Rectf m_Shape{ 106.f, 304.f, 30.f, 30.f };
		int m_SelectedIndex = 0;

		bool m_WasDownKey = false;
		bool m_WasUpKey = false;
	};
}

