#pragma once
#include <SDL.h>
#include "Singleton.h"

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
		void RenderTexture(const Texture2D& texture, const float x, const float y, int width, int height, int idx, int scale, int originalWidth) const;
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

		void DrawLine(const glm::vec2& p1, const glm::vec2& p2, const SDL_Color& color = { 255, 255, 255, 255 }) const
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

		void DrawTriangle(const glm::vec2& center, float size, const SDL_Color& color = { 255, 255, 255, 255 }) const
		{
			// Calculate the height of the equilateral triangle
			float height = (sqrt(3.0f) * 0.5f) * size;

			// Calculate the three vertices of the equilateral triangle
			//glm::vec2 p1(center.x, center.y - (2.0f / 3.0f) * height); // Top vertex
			//glm::vec2 p2(center.x - size * 0.5f, center.y + (1.0f / 3.0f) * height); // Bottom left vertex
			//glm::vec2 p3(center.x + size * 0.5f, center.y + (1.0f / 3.0f) * height); // Bottom right vertex

			glm::vec2 p1(center.x - size * 0.5f, center.y - (2.0f / 3.0f) * height); // Top vertex
			glm::vec2 p2(center.x - size * 0.5f, center.y + (2.0f / 3.0f) * height); // Bottom left vertex
			glm::vec2 p3(center.x + size * 0.5f, center.y); // Bottom right vertex

			// Call the original DrawTriangle function to draw the triangle
			DrawTriangle(p1, p2, p3, color);
		}

		void DrawTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const SDL_Color& color = { 255, 255, 255, 255 }) const
		{
			// Set the drawing color to the given color
			SDL_SetRenderDrawColor(m_RendererPtr, color.r, color.g, color.b, color.a);

			// Draw the three lines connecting the vertices of the triangle
			SDL_RenderDrawLine(m_RendererPtr, static_cast<int>(p1.x), static_cast<int>(p1.y), static_cast<int>(p2.x), static_cast<int>(p2.y));
			SDL_RenderDrawLine(m_RendererPtr, static_cast<int>(p2.x), static_cast<int>(p2.y), static_cast<int>(p3.x), static_cast<int>(p3.y));
			SDL_RenderDrawLine(m_RendererPtr, static_cast<int>(p3.x), static_cast<int>(p3.y), static_cast<int>(p1.x), static_cast<int>(p1.y));
		}

		void DrawFilledTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const SDL_Color& color = { 255, 255, 255, 255 }) const
		{
			// Set the drawing color to the given color
			SDL_SetRenderDrawColor(m_RendererPtr, color.r, color.g, color.b, color.a);

			// Helper function to draw a horizontal line between two x-coordinates on the same y-coordinate
			auto drawHorizontalLine = [&](int y, int x1, int x2)
				{
					if (x1 > x2) std::swap(x1, x2);  // Ensure x1 <= x2
					for (int x = x1; x <= x2; ++x)
					{
						SDL_RenderDrawPoint(m_RendererPtr, x, y);
					}
				};

			// Assume p1 and p2 have the same x coordinate (left points), and p3 is the right point
			float invSlope1 = (p3.y - p1.y) != 0 ? (p3.x - p1.x) / (p3.y - p1.y) : 0; // Slope between p1 and p3
			float invSlope2 = (p3.y - p2.y) != 0 ? (p3.x - p2.x) / (p3.y - p2.y) : 0; // Slope between p2 and p3

			// Determine the top and bottom left points (p1 is top, p2 is bottom if p1.y < p2.y)
			glm::vec2 top = (p1.y < p2.y) ? p1 : p2;
			glm::vec2 bottom = (p1.y < p2.y) ? p2 : p1;

			// Draw the triangle line by line using the scanline algorithm
			for (int y = static_cast<int>(top.y); y <= static_cast<int>(bottom.y); ++y)
			{
				// Compute the x-coordinates of the current scanline intersections
				float x1 = top.x + (y - top.y) * invSlope1; // Intersection with p1-p3 edge
				float x2 = bottom.x + (y - bottom.y) * invSlope2; // Intersection with p2-p3 edge

				// Draw horizontal line between the two x-coordinates
				drawHorizontalLine(y, static_cast<int>(x1), static_cast<int>(x2));
			}
		}

		void DrawFilledTriangle(const glm::vec2& center, float size, const SDL_Color& color = { 255, 255, 255, 255 }) const
		{
			// The two left points are vertically aligned, and the right point is centered between them
			glm::vec2 p1(center.x - size * 0.5f, center.y - size * 0.5f); // Top-left point
			glm::vec2 p2(center.x - size * 0.5f, center.y + size * 0.5f); // Bottom-left point
			glm::vec2 p3(center.x + size * 0.5f, center.y);            // Right point (middle y)

			// Call the filled triangle function with the calculated vertices
			const std::vector<SDL_Vertex> verts = 
			{
				{ SDL_FPoint{ p1.x, p1.y }, color, SDL_FPoint{ 0 }, },
				{ SDL_FPoint{ p2.x, p2.y }, color, SDL_FPoint{ 0 }, },
				{ SDL_FPoint{ p3.x, p3.y }, color, SDL_FPoint{ 0 }, },
			};
			DrawFilledTriangle(verts);
		}

		void DrawFilledTriangle(const std::vector<SDL_Vertex>& verts) const
		{
			SDL_RenderGeometry(m_RendererPtr, nullptr, verts.data(), static_cast<int>(verts.size()), nullptr, 0);
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

