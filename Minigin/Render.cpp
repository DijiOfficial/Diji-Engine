#include "Render.h"

#include "Transform.h"
#include "Texture.h"
#include "Text.h"
#include "Renderer.h"
#include "GameObject.h"

//temp
#include "Command.h"
#include "Collider.h"
diji::Render::Render(GameObject* ownerPtr, int scale) 
	: Render(ownerPtr)
{
	m_Scale = scale;
}

diji::Render::Render(GameObject* ownerPtr, ShapeInfo shape)
	: Component(ownerPtr)
	, m_ShapeInfo{ shape }
	, m_IsShape{ true }
{
}

diji::Render::Render(GameObject* ownerPtr)
	: Component(ownerPtr)
{
	m_TransformCompPtr = nullptr;
	m_TexturePtr = nullptr;
	m_TextureCompPtr = nullptr;
	m_TextCompPtr = nullptr;
}

void diji::Render::Init()
{
	if (m_IsShape)
		return;

	const auto& ownerPtr = GetOwner();

	m_TransformCompPtr = ownerPtr->GetComponent<Transform>();
	m_TextureCompPtr = ownerPtr->GetComponent<Texture>();
	m_TextCompPtr = ownerPtr->GetComponent<Text>();

	if (m_TextureCompPtr)
		m_TexturePtr = m_TextureCompPtr->GetTexturePtr();
	else
		m_TexturePtr = m_TextCompPtr->GetTexture();
}

void diji::Render::RenderFrame() const
{
	if (not m_Render)
		return;

	if (m_IsShape)
	{
		Renderer::GetInstance().DrawRect(m_ShapeInfo.rect, m_ShapeInfo.color);
		return;
	}

	const glm::vec3 pos = [this]()
	{
		if (m_TransformCompPtr)
			return m_TransformCompPtr->GetPosition();
		else
			return glm::vec3{ 0, 0, 0 };
	}();

	if (m_TextureCompPtr and m_TextureCompPtr->IsAnimated())
	{
		if (m_TextureCompPtr->CanRotate())
			Renderer::GetInstance().RenderRotatedTexture(*m_TexturePtr, pos.x, pos.y, m_TextureCompPtr->GetWidth(), m_TextureCompPtr->GetHeight(), m_TextureCompPtr->GetFrame(), m_TextureCompPtr->GetRotationAngle(), m_Scale);
		else
			Renderer::GetInstance().RenderTexture(*m_TexturePtr, pos.x, pos.y, m_TextureCompPtr->GetWidth(), m_TextureCompPtr->GetHeight(), m_TextureCompPtr->GetFrame(), m_Scale);
	}
	else
		Renderer::GetInstance().RenderTexture(*m_TexturePtr, pos.x, pos.y, m_Scale);

	if (m_DisplayHitbox)
	{
		Renderer::GetInstance().DrawRect(GetOwner()->GetComponent<Collider>()->GetCollisionBox());
		const auto& collision = Collision::GetInstance().GetLevelCollider();

		for (const auto& rect : collision)
			Renderer::GetInstance().DrawPolygon(rect);
	}
}

void diji::Render::UpdateText()
{ 
	m_TexturePtr = m_TextCompPtr->GetTexture(); 
}