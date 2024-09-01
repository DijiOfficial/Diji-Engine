#include "CustomTextRender.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Transform.h"
#include "Text.h"
#include "EnterName.h"
#include "Texture.h"

#include "ResourceManager.h"
#include "ScoreBoard.h"
#include "Intro.h"
#include "TimeSingleton.h"
#include "PickUp.h"
#include "Observers.h"

//add regions
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

pacman::HighScoreRender::HighScoreRender(diji::GameObject* ownerPtr, int scale)
	: Render(ownerPtr, scale)
{
    const auto& scoreboard = ScoreBoard::GetInstance().GetScoreboard();

    int count = 0;
    for (auto it = scoreboard.begin(); it != scoreboard.end() && count < 10; ++it, ++count)
    {
        m_ScoreboardMap.insert(*it);
    }
}

void pacman::HighScoreRender::Init()
{
    m_TransformCompPtr = GetOwner()->GetComponent<diji::Transform>();
    const auto& mediumFont = diji::ResourceManager::GetInstance().LoadFont("emulogic.ttf", 18);

    // Reverse iterator to start with the highest score
    auto rbegin = m_ScoreboardMap.rbegin();
    auto rend = m_ScoreboardMap.rend();

    int rank = 1;
    size_t colorIndex = 0; // Index for cycling through colors vector
    for (auto it = rbegin; it != rend && rank <= 10; ++it, ++rank)
    {
        const auto& scores = *it;

        std::string rankString;
        if (rank == 1)
            rankString = "1st";
        else if (rank == 2)
            rankString = "2nd";
        else if (rank == 3)
            rankString = "3rd";
        else
            rankString = std::to_string(rank) + "th";

        // Add space at the beginning if the rank is less than 10
        if (rank < 10)
            rankString = " " + rankString;

        // Format: "1st \tName\tScore"
        std::string formattedText = rankString + "\t" + scores.second + "\t" + std::to_string(scores.first);

        // Assign color from colors vector
        SDL_Color textColor = m_Colors[colorIndex];
        colorIndex = (colorIndex + 1) % m_Colors.size(); // Cycle through colors

        m_TextCompPtrVec.push_back(std::make_unique<diji::Text>(nullptr, formattedText, mediumFont, textColor, false));
    }
}

void pacman::HighScoreRender::Update()
{
	for (auto& textComp : m_TextCompPtrVec)
	{
		textComp->Update();
	}
}

void pacman::HighScoreRender::RenderFrame() const
{
    const glm::vec3 pos = [this]()
        {
            if (m_TransformCompPtr)
                return m_TransformCompPtr->GetPosition();
            else
                return glm::vec3{ 0, 0, 0 };
        }();

    const float startY = pos.y;
    const int yIncrement = 30;
    float currentY = startY;
    for (const auto& textComp : m_TextCompPtrVec)
    {
        if (textComp && textComp->GetTexture())
        {
            diji::Renderer::GetInstance().RenderTexture(*textComp->GetTexture(), pos.x, currentY, m_Scale);
            currentY += yIncrement;
        }
    }
}

void pacman::IntroRender::Init()
{
    m_TransformCompPtr = GetOwner()->GetComponent<diji::Transform>();
    m_IntroCompPtr = GetOwner()->GetComponent<pacman::Intro>();

    const auto& textFont = diji::ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);
    const auto& smallFont = diji::ResourceManager::GetInstance().LoadFont("emulogic.ttf", 10);
    const auto& text = m_IntroCompPtr->GetGhostNames();
    for (size_t idx{ 0 }; idx < text.size(); ++idx)
	{
        m_NamesTextCompUPtrVec.push_back(std::make_unique<diji::Text>(nullptr, text[idx].first, textFont, m_Colors[idx], false));
        m_NicknamesTextCompUPtrVec.push_back(std::make_unique<diji::Text>(nullptr, text[idx].second, textFont, m_Colors[idx], false));
	}

    m_FirstPointUPtr = std::make_unique<diji::Text>(nullptr, "10", textFont);
    m_SecondPointUPtr = std::make_unique<diji::Text>(nullptr, "50", textFont);
    m_PointsUPtr = std::make_unique<diji::Text>(nullptr, "PTS", smallFont);
}

void pacman::IntroRender::Update()
{
    //todo: put it at the end and early return when something is at max??
    m_Timer += diji::TimeSingleton::GetInstance().GetDeltaTime();
    
    if (not m_IsTextRendered and m_Timer >= 0.f)
    {
        for (const auto& textRenderComp : m_TextsRenderCompPtrVec)
            textRenderComp->EnableRender();

        m_IsTextRendered = true;
    }

    float waitTime = 1.0f;
    if (m_GhostIndex > m_NicknameIndex and m_NameIndex > m_NicknameIndex)
        waitTime = 0.5f;
    
    if (m_NicknameIndex == 5 and m_Timer >= waitTime * 0.5f)
    {
        Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::MENU_ANIMATION_BEGIN));
        ++m_NicknameIndex;
    }

    if (m_Timer >= waitTime)
    {
		m_Timer -= waitTime;

        if (m_GhostIndex == m_NameIndex)
        {
            if (m_GhostIndex == m_NicknameIndex)
            {
                if (m_GhostIndex < 4)
                    m_GhostRenderCompPtrVec[m_GhostIndex]->EnableRender();
                else if (m_GhostIndex == 4)
                {
                    m_GhostRenderCompPtrVec[m_GhostIndex]->EnableRender();
                    Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::MENU_BEGIN));
                }
                
                ++m_GhostIndex;
            }
            else
				m_NicknameIndex++;
        }
        else
        {
			++m_NameIndex;
            if (m_NameIndex == 5)
            {
                Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::MENU_BEGIN_TWO));
                m_GhostRenderCompPtrVec[m_NameIndex]->EnableRender();
            }
        }
	}

    for (size_t idx{ 0 }; idx < m_NamesTextCompUPtrVec.size(); ++idx)
    {
        m_NamesTextCompUPtrVec[idx]->Update();
        m_NicknamesTextCompUPtrVec[idx]->Update();
    }

    m_FirstPointUPtr->Update();
    m_SecondPointUPtr->Update();
    m_PointsUPtr->Update();
}

void pacman::IntroRender::Reset()
{
    m_NameIndex = 0;
	m_NicknameIndex = 0;
	m_GhostIndex = 0;
	m_Timer = -1.f;
    m_IsTextRendered = false;

    for (auto& textRenderComp : m_TextsRenderCompPtrVec)
		textRenderComp->DisableRender();

    for (auto& ghostRender : m_GhostRenderCompPtrVec)
    {
		ghostRender->DisableRender();
	}

    m_NamesTextCompUPtrVec.clear();
    m_NicknamesTextCompUPtrVec.clear();
    const auto& textFont = diji::ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);
    const auto& text = m_IntroCompPtr->GetGhostNames();
    for (size_t idx{ 0 }; idx < text.size(); ++idx)
    {
        m_NamesTextCompUPtrVec.push_back(std::make_unique<diji::Text>(nullptr, text[idx].first, textFont, m_Colors[idx], false));
        m_NicknamesTextCompUPtrVec.push_back(std::make_unique<diji::Text>(nullptr, text[idx].second, textFont, m_Colors[idx], false));
    }
}

void pacman::IntroRender::RenderFrame() const
{
    const glm::vec3 pos = [this]()
    {
        if (m_TransformCompPtr)
            return m_TransformCompPtr->GetPosition();
        else
            return glm::vec3{ 0, 0, 0 };
    }();

    const float startY = pos.y;
    const int yIncrement = 50;
    float currentY = startY;
    for (size_t idx{ 0 }; idx < m_NamesTextCompUPtrVec.size(); ++idx)
    {
        const auto& nameTextComp = m_NamesTextCompUPtrVec[idx];
        if (idx < m_NameIndex)
        {
            diji::Renderer::GetInstance().RenderTexture(*nameTextComp->GetTexture(), pos.x, currentY, m_Scale);
        }

        if (idx < m_NicknameIndex)
        {
            diji::Renderer::GetInstance().RenderTexture(*m_NicknamesTextCompUPtrVec[idx]->GetTexture(), pos.x + nameTextComp->GetTexture()->GetSize().x, currentY, m_Scale);
        }

        currentY += yIncrement;
    }

    if (m_GhostIndex > 4)
    {
	    diji::Renderer::GetInstance().RenderTexture(*m_FirstPointUPtr->GetTexture(), pos.x + 100, 400, m_Scale);
	    diji::Renderer::GetInstance().RenderTexture(*m_SecondPointUPtr->GetTexture(), pos.x + 100, 437, m_Scale);
        diji::Renderer::GetInstance().RenderTexture(*m_PointsUPtr->GetTexture(), pos.x + 149, 406, m_Scale);
        diji::Renderer::GetInstance().RenderTexture(*m_PointsUPtr->GetTexture(), pos.x + 149, 443, m_Scale);
    }
}