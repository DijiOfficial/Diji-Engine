#pragma once
#include "GameObject.h"

namespace diji
{
	class GameObject;

	class Texture final : public Component
	{
	public:
		Texture(GameObject* ownerPtr);
		Texture(GameObject* ownerPtr, const std::string& filename);
		virtual ~Texture() = default;

		Texture(const Texture& other) = delete;
		Texture(Texture&& other) = delete;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) = delete;

		void Update() override;

		void SetTexture(const std::string& filename);
		std::shared_ptr<Texture2D> GetTexture() const { return m_TexturePtr; };

	private:
		std::shared_ptr<Texture2D> m_TexturePtr{};
	};
}

