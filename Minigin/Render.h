#pragma once
#include "Component.h"

namespace diji
{
	class Texture2D;
	class Transform;
	class Texture;
	class Text;

	class Render final : public Component
	{
	public:
		Render(GameObject* ownerPtr);
		Render(GameObject* ownerPtr, int scale);
		~Render() noexcept override = default;

		Render(const Render& other) = delete;
		Render(Render&& other) = delete;
		Render& operator=(const Render& other) = delete;
		Render& operator=(Render&& other) = delete;

		void Init() override;
		void Update() override {};
		void FixedUpdate() override {};

		void RenderFrame() const;
		void UpdateText();
		void DisableRender() { m_Render = false; }
		void EnableRender() { m_Render = true; }
		void EnableHitbox() { m_DisplayHitbox = true; }
		void DisableHitbox() { m_DisplayHitbox = false; }
		void SetTestBool(bool test) { testBool = test; }
		
		void UpdateTexture(Texture2D* texture) { m_TexturePtr = texture; };
	private:
		Texture2D* m_TexturePtr;
		Texture* m_TextureCompPtr;
		Text* m_TextCompPtr;
		Transform* m_TransformCompPtr;

		int m_Scale{ 1 };
		bool m_Render{ true };
		bool m_DisplayHitbox{ false };
		bool testBool{ false };
	};
}