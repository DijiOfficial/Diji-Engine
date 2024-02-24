#include "Texture.h"
#include "Font.h"
#include "Texture2D.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdexcept>

#include "GameObject.h" //circular dependency???

Texture::Texture()
	: m_FontPtr{ nullptr }
	, m_Text{ "" }
	, m_TexturePtr{ nullptr }
	, m_needsUpdate{ false }
{
}

Texture::Texture(const std::string& filename)
	: m_FontPtr{ nullptr }
	, m_Text{ "" }
	, m_TexturePtr{ nullptr }
	, m_needsUpdate{ false }
{
		SetTexture(filename);
}

Texture::Texture(const std::string& text, std::shared_ptr<Font> font)
	: m_FontPtr{ std::move(font)}
	, m_Text{ text }
	, m_TexturePtr{ nullptr }
	, m_needsUpdate{ true }
{
}

void Texture::SetTexture(const std::string& filename)
{
	m_TexturePtr = ResourceManager::GetInstance().LoadTexture(filename);
}

void Texture::FontUpdate()
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
		m_TexturePtr = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void Texture::Update(GameObject& gameObject)
{
	FontUpdate();

	if (gameObject.HasComponent<FPSCounter>())
	{
		float fps = gameObject.GetComponent<FPSCounter>()->GetFPS();

		std::ostringstream oss;
		oss << std::fixed << std::setprecision(1) << fps << " FPS";
		std::string fpsString = oss.str();
		
		SetText(fpsString);
	}
}

void Texture::Render() const
{
	if (m_TexturePtr != nullptr)
	{
		const auto& pos = m_transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_TexturePtr, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void Texture::SetText(const std::string& text)
{
	m_Text = text;
	m_needsUpdate = true;
}

void Texture::SetFont(std::shared_ptr<Font> font)
{
	m_FontPtr = std::move(font);
	m_needsUpdate = true;
}

void Texture::SetPosition(const float x, const float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}


