#pragma once
#include "Component.h"

namespace diji
{
	class GameObject;

	class Texture final : public Component
	{
	public:
		Texture();
		Texture(const std::string& filename);
		virtual ~Texture() = default;

		Texture(const Texture& other) = default;
		Texture(Texture&& other) = default;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) = delete;

		virtual void Update(GameObject& gameObject) override;
		void Render(const GameObject& gameObject) const override;

		void SetTexture(const std::string& filename);

	private:
		std::shared_ptr<Texture2D> m_TexturePtrPtr;
	};
}

