#pragma once
#include "Component.h"
namespace diji 
{
	class GameObject;

	class Text final : public Component
	{ 
	public:
		Text();
		Text(const std::string& text, std::shared_ptr<Font> font);
		virtual ~Text() = default;

		Text(const Text& other) = default;
		Text(Text&& other) = default;
		Text& operator=(const Text& other) = delete;
		Text& operator=(Text&& other) = delete;

		virtual void Update(GameObject& gameObject) override;
		void Render(const GameObject& gameObject) const override;

		void SetText(const std::string& text);
		void SetFont(std::shared_ptr<Font> font);

	private:
		bool m_needsUpdate;
		std::string m_Text;
		std::shared_ptr<Font> m_FontPtr;
		std::shared_ptr<Texture2D> m_TexturePtrPtr;

		void FontUpdate();
	};
}
