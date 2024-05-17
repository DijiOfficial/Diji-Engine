#pragma once
#include "GameObject.h"
#include <SDL_opengl.h>
#include "Collision.h"

namespace diji
{
	class Texture2D;
	class Texture;
	class Text;
	class Transform;

	class Render final : public Component
	{
	public:
		Render(GameObject* ownerPtr);
		Render(GameObject* ownerPtr, int scale);
		~Render() override = default;

		Render(const Render& other) = delete;
		Render(Render&& other) = delete;
		Render& operator=(const Render& other) = delete;
		Render& operator=(Render&& other) = delete;

		void Update() override;
		void FixedUpdate() override {};
		void RenderFrame() const;
		void UpdateText();
		void DisableRender() { m_Render = false; }
		void EnableHitbox() { m_DisplayHitbox = true; }
		void DisableHitbox() { m_DisplayHitbox = false; }
		void SetRect(const Rectf& rect) { tempRect = rect; }
		void SetTestBool(bool test) { testBool = test; }
	private:
		Texture2D* m_TexturePtr{};
		Texture* m_TextureCompPtr{};
		Text* m_TextCompPtr{};
		Transform* m_TransformCompPtr{};

		int m_Scale{ 1 };
		bool m_Render{ true };
		bool m_DisplayHitbox{ false };
		bool testBool{ false };
		Rectf tempRect{};

		void DrawPolygon(const std::vector<glm::vec2>& vertices, bool closed = true, float lineWidth = 1.0f) const
		{
			DrawPolygon(vertices.data(), vertices.size(), closed, lineWidth);
		}

		void DrawPolygon(const glm::vec2* pVertices, size_t nrVertices, bool closed, float lineWidth = 1.0f) const
		{
			glLineWidth(lineWidth);
			closed ? glBegin(GL_LINE_LOOP) : glBegin(GL_LINE_STRIP);
			{
				for (size_t idx{ 0 }; idx < nrVertices; ++idx)
				{
					glVertex2f(pVertices[idx].x, pVertices[idx].y);
				}
			}
			glEnd();
		}

		void DrawRect(float left, float bottom, float width, float height, float lineWidth = 1.0f) const
		{
			if (width > 0 && height > 0 && lineWidth > 0)
			{

				glLineWidth(lineWidth);
				glBegin(GL_LINE_LOOP);
				{
					glVertex2f(left, bottom);
					glVertex2f(left + width, bottom);
					glVertex2f(left + width, bottom + height);
					glVertex2f(left, bottom + height);
				}
				glEnd();
			}
		}

		void DrawRect(const glm::vec2& bottomLeft, float width, float height, float lineWidth = 1.0f) const
		{
			DrawRect(bottomLeft.x, bottomLeft.y, width, height, lineWidth);
		}

		void DrawRect(const Rectf& rect, float lineWidth = 1.0f) const
		{
			DrawRect(rect.left, rect.bottom, rect.width, rect.height, lineWidth);
		}
	};
}