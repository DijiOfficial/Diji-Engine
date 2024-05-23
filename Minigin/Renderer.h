#pragma once
#include <SDL.h>
#include "Singleton.h"

//todo : temp debug stuff
#include "Collision.h"
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

		void RenderTexture(const Texture2D& texture, const float x, const float y, int scale = 1);
		void RenderTexture(const Texture2D& texture, float x, float y, int width, int height) const;
		void RenderTexture(const Texture2D& texture, float x, float y, int width, int height, int idx, int scale) const;
		void RenderRotatedTexture(const Texture2D& texture, float x, float y, int width, int height, int idx, float angle, int scale = 1) const;

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

		void DrawRect(const Rectf& rect) const
		{
			SDL_Rect rect2 = { static_cast<int>(rect.left), static_cast<int>(rect.bottom), static_cast<int>(rect.width), static_cast<int>(rect.height )}; // (x, y, width, height)
			SDL_SetRenderDrawColor(m_RendererPtr, 255, 0, 0, 255); // Set color to green
			SDL_RenderDrawRect(m_RendererPtr, &rect2);
		}

		void DrawRect(const Rectf& rect, const SDL_Color& color) const
		{
			SDL_Rect rect2 = { static_cast<int>(rect.left), static_cast<int>(rect.bottom), static_cast<int>(rect.width), static_cast<int>(rect.height) }; // (x, y, width, height)
			SDL_SetRenderDrawColor(m_RendererPtr, color.r, color.g, color.b, color.a); // Set color to green
			SDL_RenderDrawRect(m_RendererPtr, &rect2);
		}

		void DrawRect(const SDL_Rect& rect) const
		{
			//SDL_Rect rect2 = { rect.left, rect.bottom, rect.width, rect.height }; // (x, y, width, height)
			SDL_SetRenderDrawColor(m_RendererPtr, 0, 255, 0, 255); // Set color to green
			SDL_RenderDrawRect(m_RendererPtr, &rect);
		}

		void DrawLine(const glm::vec2& p1, const glm::vec2& p2, const SDL_Color& color) const
		{
			SDL_SetRenderDrawColor(m_RendererPtr, color.r, color.g, color.b, color.a);
			SDL_RenderDrawLine(m_RendererPtr, static_cast<int>(p1.x), static_cast<int>(p1.y), static_cast<int>(p2.x), static_cast<int>(p2.y));
		}

		void DrawPolygon(const std::vector<glm::vec2>& points)
		{
			SDL_SetRenderDrawColor(m_RendererPtr, 255, 0, 0, 255); // Set color to green

			// Convert glm::vec2 points to SDL_Point
			std::vector<SDL_Point> sdlPoints;
			for (const auto& point : points)
			{
				sdlPoints.push_back({ static_cast<int>(point.x), static_cast<int>(point.y) });
			}

			// Ensure the polygon is closed by adding the first point at the end
			if (!sdlPoints.empty())
			{
				sdlPoints.push_back(sdlPoints.front());
			}

			// Draw the lines
			SDL_RenderDrawLines(m_RendererPtr, sdlPoints.data(), static_cast<int>(sdlPoints.size()));
		}

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

	private:
		SDL_Window* m_WindowPtr = nullptr;
		SDL_Renderer* m_RendererPtr = nullptr;
		SDL_Color m_ClearColor = { 0, 0, 0, 0 };
	};
}

