#pragma once
#include "Render.h"

namespace pacman 
{
	class EnterName;

	class CustomTextRender final : public diji::Render
	{
	public:
		CustomTextRender(diji::GameObject* ownerPtr) : Render(ownerPtr) {};
		~CustomTextRender() noexcept override = default;

		void Init() override;
		void Update() override {};
		void FixedUpdate() override {};

		void RenderFrame() const override;	
		virtual void UpdateText();
	private:
		diji::Texture2D* m_TextureCompPtr = nullptr;
		diji::Transform* m_TransformCompPtr = nullptr;
		EnterName* m_EnterNameCompPtr = nullptr;

		float m_LetterWidth = 0;
		float m_LetterHeight = 0;

		void DrawCustomArrow(const glm::vec3& pos) const;
	};
}

