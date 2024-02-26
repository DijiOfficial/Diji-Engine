#pragma once
#include "Component.h"

namespace diji
{
	class GameObject;

	class Texture final : public Component
	{
	public:
		Texture(GameObject* ownerPtr);
		Texture(const std::string& filename, GameObject* ownerPtr);
		virtual ~Texture() = default;

		Texture(const Texture& other) = delete;
		Texture(Texture&& other) = delete;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) = delete;

		virtual void Update() override;

		void SetTexture(const std::string& filename);

	private:
		std::shared_ptr<Texture2D> m_TexturePtr;
	};
}

