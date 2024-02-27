#pragma once
#include "GameObject.h"

namespace diji 
{
	class GameObject;
	class Font;
	class Texture2D;

	class Text final : public Component
	{ 
	public:
		Text(GameObject* ownerPtr);
		Text(GameObject* ownerPtr, const std::string& text, std::shared_ptr<Font> font);
		~Text() override = default;

		Text(const Text& other) = delete;
		Text(Text&& other) = delete;
		Text& operator=(const Text& other) = delete;
		Text& operator=(Text&& other) = delete;

		void Update() override;

		void SetText(const std::string& text);
		void SetFont(std::shared_ptr<Font> font);
		std::shared_ptr<Texture2D> GetTexture() const { return m_TexturePtr; };
		bool GetIsDirty() const { return m_IsDirty; };
		void SetClean() { m_IsDirty = false; };

	private:
		bool m_NeedsUpdate;
		bool m_IsDirty;
		std::string m_Text;
		std::shared_ptr<Font> m_FontPtr;
		std::shared_ptr<Texture2D> m_TexturePtr;

		void FontUpdate();
	};
}
