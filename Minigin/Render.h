#pragma once
#include "Component.h"
#include <SDL.h>
#include "Collision.h"

namespace diji
{
	class Texture2D;
	class Transform;
	class Texture;
	class Text;

	struct ShapeInfo
	{
		Rectf rect = {0, 0, 0, 0};
		SDL_Color color = { 255, 255, 255, 255 };
		int lineWidth = 1;
	};

	class Render : public Component
	{
	public:
		Render(GameObject* ownerPtr);
		Render(GameObject* ownerPtr, int scale);
		Render(GameObject* ownerPtr, ShapeInfo shape);
		~Render() noexcept override = default;

		Render(const Render& other) = delete;
		Render(Render&& other) = delete;
		Render& operator=(const Render& other) = delete;
		Render& operator=(Render&& other) = delete;

		void Init() override;
		void Update() override {};
		void FixedUpdate() override {};

		virtual void RenderFrame() const;
		virtual void UpdateText();
		virtual void UpdateTexture(Texture2D* texture) { m_TexturePtr = texture; };
		void DisableRender() { m_Render = false; }
		void EnableRender() { m_Render = true; }
		void SetIsShape() { m_IsShape = true; }
		void UpdateShape(const Rectf& shape) { m_ShapeInfo.rect = shape; }
	protected:
		int m_Scale = 1;
		bool m_Render = true;
	private:
		Transform* m_TransformCompPtr;
		Texture2D* m_TexturePtr;
		Texture* m_TextureCompPtr;
		Text* m_TextCompPtr;

		ShapeInfo m_ShapeInfo;

		bool m_IsShape = false;
	};
}