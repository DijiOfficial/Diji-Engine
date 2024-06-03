#include "CustomTextRender.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Transform.h"
#include "Text.h"
#include "EnterName.h"
#include "Texture.h"

void pacman::CustomTextRender::Init()
{
	m_TransformCompPtr = GetOwner()->GetComponent<diji::Transform>();
	m_TextureCompPtr = GetOwner()->GetComponent<diji::Text>()->GetTexture();
    m_EnterNameCompPtr = GetOwner()->GetComponent<pacman::EnterName>();
}

void pacman::CustomTextRender::RenderFrame() const
{
	const glm::vec3 pos = [this]()
	{
		if (m_TransformCompPtr)
			return m_TransformCompPtr->GetPosition();
		else
			return glm::vec3{ 0, 0, 0 };
	}();

	diji::Renderer::GetInstance().RenderTexture(*m_TextureCompPtr, pos.x, pos.y);

	DrawCustomArrow(pos);
}

void pacman::CustomTextRender::UpdateText()
{
	m_TextureCompPtr = GetOwner()->GetComponent<diji::Text>()->GetTexture();
    m_LetterHeight = static_cast<float>(m_TextureCompPtr->GetSize().y);
    m_LetterWidth = m_TextureCompPtr->GetSize().x / 3.f;
}

void pacman::CustomTextRender::DrawCustomArrow(const glm::vec3& pos) const
{
    constexpr float arrowLength = 10.0f; 
    constexpr float arrowSpacing = 5.0f;

    const int currentIndex = m_EnterNameCompPtr->GetCurrentIndex();
    const float currentLetterX = pos.x + (currentIndex + 0.5f) * m_LetterWidth;
    const float currentLetterY = pos.y;

    const glm::vec2 upArrowP2 = { currentLetterX, currentLetterY - arrowSpacing - arrowLength };
    const glm::vec2 upArrowP3 = { currentLetterX - arrowLength * 0.5f, currentLetterY - arrowSpacing - arrowLength * 0.5f };
    const glm::vec2 upArrowP4 = { currentLetterX + arrowLength * 0.5f, currentLetterY - arrowSpacing - arrowLength * 0.5f };
    const glm::vec2 downArrowP2 = { currentLetterX, currentLetterY + m_LetterHeight + arrowSpacing + arrowLength };
    const glm::vec2 downArrowP3 = { currentLetterX - arrowLength * 0.5f, currentLetterY + m_LetterHeight + arrowSpacing + arrowLength * 0.5f };
    const glm::vec2 downArrowP4 = { currentLetterX + arrowLength * 0.5f, currentLetterY + m_LetterHeight + arrowSpacing + arrowLength * 0.5f };

    for (int i = 0; i <= 4; ++i)
    {
        glm::vec2 upArrowP2_thick = { upArrowP2.x, upArrowP2.y + i };
        glm::vec2 upArrowP3_thick = { upArrowP3.x, upArrowP3.y + i };
        glm::vec2 upArrowP4_thick = { upArrowP4.x, upArrowP4.y + i };
        glm::vec2 downArrowP2_thick = { downArrowP2.x, downArrowP2.y + i };
        glm::vec2 downArrowP3_thick = { downArrowP3.x, downArrowP3.y + i };
        glm::vec2 downArrowP4_thick = { downArrowP4.x, downArrowP4.y + i };

        diji::Renderer::GetInstance().DrawLine(upArrowP2_thick, upArrowP3_thick);
        diji::Renderer::GetInstance().DrawLine(upArrowP2_thick, upArrowP4_thick);
        diji::Renderer::GetInstance().DrawLine(downArrowP2_thick, downArrowP3_thick);
        diji::Renderer::GetInstance().DrawLine(downArrowP2_thick, downArrowP4_thick);
    }
}

void pacman::LevelCounterRender::Init()
{
    m_TransformCompPtr = GetOwner()->GetComponent<diji::Transform>();
    m_TextureCompPtr = GetOwner()->GetComponent<diji::Texture>();
    m_Texture2DCompPtr = m_TextureCompPtr->GetTexturePtr();
}

void pacman::LevelCounterRender::RenderFrame() const
{
    const glm::vec3 pos = [this]()
    {
        if (m_TransformCompPtr)
            return m_TransformCompPtr->GetPosition();
        else
            return glm::vec3{ 0, 0, 0 };
    }();
    
    diji::Renderer::GetInstance().RenderTexture(*m_Texture2DCompPtr, pos.x, pos.y, m_TextureCompPtr->GetWidth(), m_TextureCompPtr->GetHeight(), m_TextureCompPtr->GetFrame(), m_Scale, 16);
}

void pacman::FruitRender::Init()
{
    m_TextCompPtr = GetOwner()->GetComponent<diji::Text>();
    m_TransformCompPtr = GetOwner()->GetComponent<diji::Transform>();
    m_TextureCompPtr = GetOwner()->GetComponent<diji::Texture>();
    m_Texture2DCompPtr = m_TextureCompPtr->GetTexturePtr();
    m_Text2DCompPtr = m_TextCompPtr->GetTexture();
}

void pacman::FruitRender::RenderFrame() const
{
    if (not m_Render)
        return;

    const glm::vec3 pos = 
    [this]()
    {
        if (m_TransformCompPtr)
            return m_TransformCompPtr->GetPosition();
        else
            return glm::vec3{ 0, 0, 0 };
    }();

    if (m_RenderText)
        diji::Renderer::GetInstance().RenderTexture(*m_Text2DCompPtr, pos.x - (m_Text2DCompPtr->GetSize().x + m_TextureCompPtr->GetWidth()) * 0.5f, pos.y + m_Text2DCompPtr->GetSize().y *0.5f, m_Scale);
	else
        diji::Renderer::GetInstance().RenderTexture(*m_Texture2DCompPtr, pos.x, pos.y, m_TextureCompPtr->GetWidth(), m_TextureCompPtr->GetHeight(), m_TextureCompPtr->GetFrame(), m_Scale, 16);
    
}

void pacman::FruitRender::UpdateText()
{
    m_Text2DCompPtr = m_TextCompPtr->GetTexture();
}
