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

		void DrawCircle(int x, int y, int radius)
		{
			SDL_SetRenderDrawColor(m_RendererPtr, 255, 255, 255, 255);
			for (int w = 0; w < radius * 2; w++)
			{
				for (int h = 0; h < radius * 2; h++)
				{
					int dx = radius - w; // horizontal offset
					int dy = radius - h; // vertical offset
					if ((dx * dx + dy * dy) <= (radius * radius))
					{
						SDL_RenderDrawPoint(m_RendererPtr, x + dx, y + dy);
					}
				}
			}
		}

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }
	
	private:
		SDL_Renderer* m_RendererPtr{};
		SDL_Window* m_WindowPtr{};
		SDL_Color m_ClearColor{};	
	};
}

