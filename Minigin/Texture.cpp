#include "Texture.h"
#include "GameObject.h"

diji::Texture::Texture(GameObject* ownerPtr)
	: Component(ownerPtr)
	, m_TexturePtr{ nullptr }
{
}

diji::Texture::Texture(const std::string& filename, GameObject* ownerPtr)
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

//void diji::Texture::Render(const GameObject& gameObject) const
//{
//	if (m_TexturePtrPtr != nullptr)
//	{
//		const auto& pos = gameObject.HasComponent<Transform>() ? gameObject.GetComponent<Transform>()->GetPosition() : glm::vec3{ 0, 0, 0 };
//		Renderer::GetInstance().RenderTexture(*m_TexturePtrPtr, pos.x, pos.y);
//	}
//}


