#pragma once
#include "Component.h"

namespace diji
{
	class Font;
	class Texture2D;
	class GameObject;

	class Texture final : public Component
	{
	public:
		Texture(const std::string& text, std::shared_ptr<Font> font);
		Texture(const std::string& filename);
		Texture();
		virtual ~Texture() = default;

		Texture(const Texture& other) = default;
		Texture(Texture&& other) = default;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) = delete;

		virtual void Update(GameObject& gameObject) override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetFont(std::shared_ptr<Font> font);
		void SetPosition(float x, float y);
		void SetTexture(const std::string& filename);

	private:
		bool m_needsUpdate;
		std::string m_Text;
		Transform m_Transform{};
		std::shared_ptr<Font> m_FontPtr;
		std::shared_ptr<Texture2D> m_TexturePtrPtr;

		void FontUpdate();
	};
}

