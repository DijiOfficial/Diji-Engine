#pragma once
#include "GameObject.h"

namespace diji 
{
	class Font;
	class Texture2D;

	class Text final : public Component
	{ 
	public:
		Text(GameObject* ownerPtr);
		Text(GameObject* ownerPtr, const std::string& text, Font* font);
		~Text() override = default;

		Text(const Text& other) = delete;
		Text(Text&& other) = delete;
		Text& operator=(const Text& other) = delete;
		Text& operator=(Text&& other) = delete;

		void Update() override;

		void SetText(const std::string& text);
		Texture2D* GetTexture() const { return m_TexturePtr.get(); };
		void SetFont(Font* fontPtr);

	private:
		std::string m_Text;
		Font* m_FontPtr;
		std::unique_ptr<Texture2D> m_TexturePtr;
		bool m_NeedsUpdate;
	};
}
