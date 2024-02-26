#include "Render.h"
#include "Transform.h"
#include "Texture.h"
#include "Text.h"

diji::Render::Render(GameObject* ownerPtr) 
	: Component(ownerPtr) 
{ 
	m_TextureCompPtr = ownerPtr->GetComponent<Texture>();
	m_TransformCompPtr = ownerPtr->GetComponent<Transform>();
	m_TextCompPtr = ownerPtr->GetComponent<Text>();
};


void diji::Render::Update()
{
	CheckNullptr();
	CheckTextUpdate();

	if (m_TexturePtr != nullptr)
	{
		const auto& pos = m_TransformCompPtr ? m_TransformCompPtr->GetPosition() : glm::vec3{ 0, 0, 0 };
		Renderer::GetInstance().RenderTexture(*m_TexturePtr, pos.x, pos.y);
	}
	else
	{
		if (m_TextureCompPtr)
			m_TexturePtr = m_TextureCompPtr->GetTexture();
		else if (m_TextCompPtr)
			m_TexturePtr = m_TextCompPtr->GetTexture();
	}
}

void diji::Render::CheckNullptr()
{
	if (not m_TextureCompPtr and GetOwner()->HasComponent<Texture>())
		m_TextureCompPtr = GetOwner()->GetComponent<Texture>();

	if (not m_TransformCompPtr and GetOwner()->HasComponent<Transform>())
		m_TransformCompPtr = GetOwner()->GetComponent<Transform>();

	if (not m_TextCompPtr and GetOwner()->HasComponent<Text>())
		m_TextCompPtr = GetOwner()->GetComponent<Text>();

}

void diji::Render::CheckTextUpdate()
{
	if (not m_TextCompPtr)
		return;

	if (m_TextCompPtr->GetIsDirty())
	{
		m_TexturePtr = m_TextCompPtr->GetTexture();
		m_TextCompPtr->SetClean();
	}
}
