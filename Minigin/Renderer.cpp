#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "GUI.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void diji::Renderer::Init(SDL_Window* window)
{
	m_WindowPtr = window;
	m_RendererPtr = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_RendererPtr == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void diji::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_RendererPtr, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_RendererPtr);

	SceneManager::GetInstance().Render();
	GUI::GetInstance().Render();
	
	SDL_RenderPresent(m_RendererPtr);
}

void diji::Renderer::Destroy()
{
	if (m_RendererPtr != nullptr)
	{
		SDL_DestroyRenderer(m_RendererPtr);
		m_RendererPtr = nullptr;
	}
}

//void diji::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
//{
//	SDL_Rect dst{};
//	dst.x = static_cast<int>(x);
//	dst.y = static_cast<int>(y);
//	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
//	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
//}

void diji::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, int scale)
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	dst.w *= scale;
	dst.h *= scale;
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void diji::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, int width, int height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = width;
	dst.h = height;
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void diji::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, int width, int height, int idx, int scale) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = width * scale;
	dst.h = height * scale;

	SDL_Rect src{};
	src.x = idx * width;
	src.y = 0;
	src.w = width;
	src.h = height;
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}

SDL_Renderer* diji::Renderer::GetSDLRenderer() const { return m_RendererPtr; }
