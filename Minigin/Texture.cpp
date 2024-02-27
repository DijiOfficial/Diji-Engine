#include "Texture.h"

diji::Texture::Texture(GameObject* ownerPtr)
	: Component(ownerPtr)
	, m_TexturePtr{ nullptr }
	, m_Width{ 0 }
	, m_Height{ 0 }
	, m_Idx{ 0 }
{
}

diji::Texture::Texture(GameObject* ownerPtr, const std::string& filename)
	: Component(ownerPtr)
	, m_TexturePtr{ nullptr }
	, m_Width{ 0 }
	, m_Height{ 0 }
	, m_Idx{ 0 }
{
		SetTexture(filename);
}

diji::Texture::Texture(GameObject* ownerPtr, const std::string& filename, int width, int height, int idx)
	: Component(ownerPtr)
	, m_TexturePtr{ nullptr }
	, m_Width{ width }
	, m_Height{ height }
	, m_Idx{ idx }
{
	SetTexture(filename);
	m_HasVariableSize = true;
}

void diji::Texture::SetTexture(const std::string& filename)
{
	m_TexturePtr = ResourceManager::GetInstance().LoadTexture(filename);
}

void diji::Texture::Update()
{
}

