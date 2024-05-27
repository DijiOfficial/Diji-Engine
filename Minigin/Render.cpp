#include "Render.h"

#include "Transform.h"
#include "Texture.h"
#include "Text.h"
#include "Renderer.h"
#include "GameObject.h"

//temp
#include "Command.h"
#include "Collision.h"
#include "Collider.h"
diji::Render::Render(GameObject* ownerPtr, int scale) 
	: Render(ownerPtr)
{
	m_Scale = scale;
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

	if (testBool)
	{
		const auto& temppos = GetOwner()->GetComponent<Transform>()->m_target;
		Renderer::GetInstance().DrawCircle((int)temppos.x, (int)temppos.y, 5);

		const auto& test2 = GetOwner()->GetComponent<Transform>()->blinky;
		Renderer::GetInstance().DrawCircle((int)test2.x, (int)test2.y, 5);

		const auto& test3 = GetOwner()->GetComponent<Transform>()->firstTarget;
		Renderer::GetInstance().DrawCircle((int)test3.x, (int)test3.y, 5);
		//auto test= GetOwner()->GetComponent<Collider>()->GetCollisionBox();

		//auto movement = m_TransformCompPtr->GetMovement();
		//glm::vec2 translation{ 0, 0 };
		//switch (movement)
		//{
		//case diji::Movement::Right:
		//	translation.x = 16;
		//	break;
		//case diji::Movement::Down:
		//	translation.y = 16;
		//	break;
		//case diji::Movement::Left:
		//	translation.x = -16;
		//	break;
		//case diji::Movement::Up:
		//	translation.y = -16;
		//	break;
		//}
		//const glm::vec2 center(test.left + test.width * 0.5f, test.bottom + test.height * 0.5f);
		//const glm::vec2 teste = center + translation;

		//Renderer::GetInstance().DrawLine(center, teste, SDL_Color{255,255,255,255});

	}
}

void diji::Render::UpdateText()
{ 
	m_TexturePtr = m_TextCompPtr->GetTexture(); 
}