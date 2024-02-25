#include "Texture.h"
#include "Font.h"
#include "Texture2D.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdexcept>

#include "GameObject.h"

diji::Texture::Texture()
	: m_FontPtr{ nullptr }
	, m_Text{ "" }
	, m_TexturePtrPtr{ nullptr }
	, m_needsUpdate{ false }
{
}

diji::Texture::Texture(const std::string& filename)
	: m_FontPtr{ nullptr }
	, m_Text{ "" }
	, m_TexturePtrPtr{ nullptr }
	, m_needsUpdate{ false }
{
		SetTexture(filename);
}

diji::Texture::Texture(const std::string& text, std::shared_ptr<Font> font)
	: m_FontPtr{ std::move(font)}
	, m_Text{ text }
	, m_TexturePtrPtr{ nullptr }
	, m_needsUpdate{ true }
{
}

void diji::Texture::SetTexture(const std::string& filename)
{
	m_TexturePtrPtr = ResourceManager::GetInstance().LoadTexture(filename);
}

void diji::Texture::FontUpdate()
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

void diji::Texture::Update(GameObject& gameObject)
{
	FontUpdate();

	if (gameObject.HasComponent<FPSCounter>())
	{
		double fps = gameObject.GetComponent<FPSCounter>()->GetFPS();

		std::ostringstream oss;
		oss << std::fixed << std::setprecision(1) << fps << " FPS";
		std::string fpsString = oss.str();
		
		SetText(fpsString);
	}
}

void diji::Texture::Render(const GameObject& gameObject) const
{
	if (m_TexturePtrPtr != nullptr)
	{
		const auto& pos = gameObject.HasComponent<Transform>() ? gameObject.GetComponent<Transform>()->GetPosition() : glm::vec3{ 0, 0, 0 };
		Renderer::GetInstance().RenderTexture(*m_TexturePtrPtr, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void diji::Texture::SetText(const std::string& text)
{
	m_Text = text;
	m_needsUpdate = true;
}

void diji::Texture::SetFont(std::shared_ptr<Font> font)
{
	m_FontPtr = std::move(font);
	m_needsUpdate = true;
}


