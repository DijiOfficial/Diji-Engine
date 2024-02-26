#include "Text.h"
#include "Font.h"

#include <SDL_ttf.h>
#include <stdexcept>

diji::Text::Text(GameObject* ownerPtr)
	: Component(ownerPtr)
	, m_FontPtr{ nullptr }
	, m_Text{ "" }
	, m_TexturePtrPtr{ nullptr }
	, m_needsUpdate{ false }
{
}

diji::Text::Text(const std::string& text, std::shared_ptr<Font> font, GameObject* ownerPtr)
	: Component(ownerPtr)
	, m_FontPtr{ std::move(font) }
	, m_Text{ text }
	, m_TexturePtrPtr{ nullptr }
	, m_needsUpdate{ true }
{
}

void diji::Text::FontUpdate()
{
	if (m_needsUpdate)
	{
		const SDL_Color color = { 255, 255, 255, 255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_FontPtr->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_TexturePtrPtr = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void diji::Text::Update()
{
	FontUpdate();
}

//void diji::Text::Render(const GameObject& gameObject) const
//{
//	if (m_TexturePtrPtr != nullptr)
//	{
//		const auto& pos = gameObject.HasComponent<Transform>() ? gameObject.GetComponent<Transform>()->GetPosition() : glm::vec3{ 0, 0, 0 };
//		Renderer::GetInstance().RenderTexture(*m_TexturePtrPtr, pos.x, pos.y);
//	}
//}

// This implementation uses the "dirty flag" pattern
void diji::Text::SetText(const std::string& text)
{
	m_Text = text;
	m_needsUpdate = true;
}

void diji::Text::SetFont(std::shared_ptr<Font> font)
{
	m_FontPtr = std::move(font);
	m_needsUpdate = true;
}



