#include "Texture.h"

diji::Texture::Texture(GameObject* ownerPtr)
	: Component(ownerPtr)
	, m_TexturePtr{ nullptr }
{
}

diji::Texture::Texture(GameObject* ownerPtr, const std::string& filename)
	: Component(ownerPtr)
	, m_TexturePtr{ nullptr }
{
		SetTexture(filename);
}

void diji::Texture::SetTexture(const std::string& filename)
{
	m_TexturePtr = ResourceManager::GetInstance().LoadTexture(filename);
}

void diji::Texture::Update()
{
}

