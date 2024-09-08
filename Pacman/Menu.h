#pragma once
#include "Component.h"
#include "Collision.h"
#include "Render.h"
#include "Transform.h"

namespace pacman
{
	enum class MenuState
	{
		//give them the value of how many textes they have
		MENU = 6,
		MULTIPLAYER = 3,
		MULTIPLAYEROPTIONS = 12,
		OPTIONS,
		HIGHSCORE,
	};
	class Menu final : public diji::Component
	{
	public:
		Menu(diji::GameObject* ownerPtr) : Component(ownerPtr) {};
		~Menu() noexcept override = default;

		void Init() override;
		void Update() override;
		void FixedUpdate() override {};

		void ValidateChoice();
		void SwitchUp();
		void SwitchDown();
		int GetSelectedIndex() const { return m_SelectedIndex; };
		void AddTextRender(const std::vector<diji::Render*>& textRenderVec) { m_TextRenderCompPtrVec = textRenderVec; };

	private:
		float m_DisplayTimer = 0;
		int m_SelectedIndex = 0;
		bool m_Display = true;
		
		//diji::Render* m_BackRenderCompPtr = nullptr;
		diji::Transform* m_BackTransformCompPtr = nullptr;
		std::vector<diji::Render*> m_TextRenderCompPtrVec;
		std::vector<diji::Render*> m_MultiplayerRenderCompPtrVec;
		std::vector<diji::Render*> m_OptionsRenderCompPtrVec;
		//std::vector<diji::Render*> m_OptionsRenderCompPtrVec;
		MenuState m_MenuState = MenuState::MENU;

		std::vector<diji::Render*>* GetCurrentStateRenderCompPtrVec();
		int GetDisplayIndex() const;
	};
}

