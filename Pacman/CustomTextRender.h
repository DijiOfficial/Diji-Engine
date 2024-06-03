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

	class LevelCounterRender final : public diji::Render
	{
	public:
		LevelCounterRender(diji::GameObject* ownerPtr, int scale = 1) : Render(ownerPtr, scale) {};
		~LevelCounterRender() noexcept override = default;

		void Init() override;
		void Update() override {};
		void FixedUpdate() override {};

		void RenderFrame() const override;
		void UpdateText() override {};
	private:
		diji::Texture* m_TextureCompPtr = nullptr;
		diji::Texture2D* m_Texture2DCompPtr = nullptr;
		diji::Transform* m_TransformCompPtr = nullptr;
	};

	class FruitRender final : public diji::Render
	{
	public:
		FruitRender(diji::GameObject* ownerPtr, int scale = 1) : Render(ownerPtr, scale) {};
		~FruitRender() noexcept override = default;

		void Init() override;
		void Update() override {};
		void FixedUpdate() override {};

		void RenderFrame() const override;
		void UpdateText() override;
		void RenderText(bool render) { m_RenderText = render; };

	private:
		diji::Text* m_TextCompPtr = nullptr;
		diji::Texture* m_TextureCompPtr = nullptr;
		diji::Texture2D* m_Texture2DCompPtr = nullptr;
		diji::Texture2D* m_Text2DCompPtr = nullptr;
		diji::Transform* m_TransformCompPtr = nullptr;

		bool m_RenderText = false;
	};
}

