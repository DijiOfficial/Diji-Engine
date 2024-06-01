#pragma once
//#include "Text.h"
#include "Component.h"
#include <string>
namespace diji 
{
	class GameObject;
	enum class Movement;
	class Text;
}

namespace pacman
{
	class ScoreCounter;
	class EnterName final : public diji::Component
	{
	public:
		EnterName(diji::GameObject* ownerPtr, diji::GameObject* player);
		~EnterName() noexcept override = default;

		void Init() override;
		void Update() override;
		void FixedUpdate() override {};

		void MoveLetter(diji::Movement movement);
		int GetCurrentIndex() const { return m_SelectedIndex; }
	
	private:
		int m_SelectedIndex = 0;
		std::string m_Name = "AAA";
		bool m_IsDirty = false;
		diji::Text* m_TextCompPtr = nullptr;
		ScoreCounter* m_ScoreCounterCompPtr = nullptr;
	};
}

