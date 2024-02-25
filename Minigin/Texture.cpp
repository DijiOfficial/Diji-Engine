#include "Texture.h"
#include "GameObject.h"

diji::Texture::Texture()
	: m_TexturePtrPtr{ nullptr }
{
}

diji::Texture::Texture(const std::string& filename)
	: m_TexturePtrPtr{ nullptr }
{
		SetTexture(filename);
}

void diji::Texture::SetTexture(const std::string& filename)
{
	m_TexturePtrPtr = ResourceManager::GetInstance().LoadTexture(filename);
}

void diji::Texture::Update(GameObject& gameObject)
{
	(void)gameObject;
}

void diji::Texture::Render(const GameObject& gameObject) const
{
	if (m_TexturePtrPtr != nullptr)
	{
		const auto& pos = gameObject.HasComponent<Transform>() ? gameObject.GetComponent<Transform>()->GetPosition() : glm::vec3{ 0, 0, 0 };
		Renderer::GetInstance().RenderTexture(*m_TexturePtrPtr, pos.x, pos.y);
	}
}


