#include "Render.h"
#include "Transform.h"
#include "Texture.h"
#include "Text.h"

#include "Rotation.h"
diji::Render::Render(GameObject* ownerPtr) 
	: Component(ownerPtr) 
{ 
	m_TransformCompPtr = ownerPtr->GetComponent<Transform>();

	m_TextureCompPtr = ownerPtr->GetComponent<Texture>();
	m_TextCompPtr = ownerPtr->GetComponent<Text>();

	if (not (m_TextureCompPtr or m_TextCompPtr))
	{
		if (m_TextCompPtr)
			assert(m_TextureCompPtr and "Texture Component needs to be initialized before Render");
		assert(m_TextCompPtr and "Text Component needs to be initialized before Render");
	}

	if (m_TextureCompPtr)
		m_TexturePtr = m_TextureCompPtr->GetTexture();
	else
		m_TexturePtr = m_TextCompPtr->GetTexture();
};

void diji::Render::Update()
{
	if (not m_TextCompPtr)
		return;

	if (m_TextCompPtr->GetIsDirty())
	{
		m_TexturePtr = m_TextCompPtr->GetTexture();
		m_TextCompPtr->SetClean();
	}
}

void diji::Render::RenderFrame() const
{
	const glm::vec3 pos = [this]()
		{
			if (m_TransformCompPtr)
				return m_TransformCompPtr->GetPosition();
			else
				return glm::vec3{ 0, 0, 0 };
		}();
		
	if(m_TextureCompPtr and m_TextureCompPtr->IsAnimated())
		Renderer::GetInstance().RenderTexture(*m_TexturePtr, pos.x, pos.y, m_TextureCompPtr->GetWidth(), m_TextureCompPtr->GetHeight(), m_TextureCompPtr->GetFrame());
	else
		Renderer::GetInstance().RenderTexture(*m_TexturePtr, pos.x, pos.y);
}
