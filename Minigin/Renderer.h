#pragma once
#include <SDL.h>
#include "Singleton.h"


namespace diji
{
	class Texture2D;
	/**
		* Simple RAII wrapper for the SDL renderer
		*/
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, int width, int height) const;
		void RenderTexture(const Texture2D& texture, float x, float y, int width, int height, int idx) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }
	
	private:
		SDL_Renderer* m_RendererPtr{};
		SDL_Window* m_WindowPtr{};
		SDL_Color m_ClearColor{};	
	};
}

