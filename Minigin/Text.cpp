#include "Text.h"
#include "Font.h"

#include <SDL_ttf.h>
#include <stdexcept>

diji::Text::Text(GameObject* ownerPtr)
	: Component(ownerPtr)
	, m_Text{ "" }
	, m_FontPtr{ nullptr }
	, m_TexturePtr{ nullptr }
	, m_NeedsUpdate{ false }
	, m_IsDirty{ false }
{
}

diji::Text::Text(GameObject* ownerPtr, const std::string& text, std::shared_ptr<Font> font)
	: Component(ownerPtr)
	, m_Text{ text }
	, m_FontPtr{ std::move(font) }
	, m_TexturePtr{ nullptr }
	, m_NeedsUpdate{ true }
	, m_IsDirty{ true }
{
}

void diji::Text::Update()
{
	if (m_NeedsUpdate)
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
		m_TexturePtr = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

// This implementation uses the "dirty flag" pattern
void diji::Text::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
	m_IsDirty = true;
}

void diji::Text::SetFont(std::shared_ptr<Font> font)
{
	m_FontPtr = std::move(font);
	m_NeedsUpdate = true;
	m_IsDirty = true;
}



