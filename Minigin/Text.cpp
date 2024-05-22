#include "Text.h"

#include "Font.h"
#include "Render.h"
#include "Transform.h"
#include "Renderer.h"
#include "GameObject.h"
#include <SDL_ttf.h>

diji::Text::Text(GameObject* ownerPtr)
	: Component(ownerPtr)
	, m_Text{ "" }
	, m_FontPtr{ nullptr }
	, m_TexturePtr{ nullptr }
	, m_Color{ 255, 255, 255, 255 }
	, m_NeedsUpdate{ false }
	, m_IsCentered{ false }
{
}

diji::Text::Text(GameObject* ownerPtr, const std::string& text, Font* font, const SDL_Color& color, bool centered)
	: Component(ownerPtr)
	, m_Text{ text }
	, m_FontPtr{ font }
	, m_TexturePtr{ nullptr }
	, m_Color{ color }
	, m_NeedsUpdate{ true }
	, m_IsCentered{ centered }
{
}

void diji::Text::Update()
{
	if (m_NeedsUpdate)
	{
		//const SDL_Color color = { 255, 255, 255, 255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_FontPtr->GetFont(), m_Text.c_str(), m_Color);
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
		m_TexturePtr = std::make_unique<Texture2D>(texture);
		m_NeedsUpdate = false;

		// need to update the pointer in Render this frame, Render update was already called so it has previous pointer information
		GetOwner()->GetComponent<Render>()->UpdateText();

		if (m_IsCentered)
		{
			const auto& pos = GetOwner()->GetComponent<Transform>()->GetPosition();
			GetOwner()->GetComponent<Transform>()->SetPosition(pos.x - m_TexturePtr->GetSize().x * 0.5f, pos.y);
		}
	}
}

// This implementation uses the "dirty flag" pattern
void diji::Text::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void diji::Text::SetFont(Font* fontPtr)
{
	m_FontPtr = fontPtr;
	m_NeedsUpdate = true;
}



