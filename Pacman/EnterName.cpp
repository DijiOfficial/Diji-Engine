#include "EnterName.h"
#include "Command.h"
#include "GameObject.h"
#include "Text.h"
#include "ScoreBoard.h"
#include "ScoreCounter.h"
#include "SceneManager.h"
#include "GameState.h"

pacman::EnterName::EnterName(diji::GameObject* ownerPtr, diji::GameObject* player)
	: diji::Component(ownerPtr)
{
	m_ScoreCounterCompPtr = player->GetComponent<ScoreCounter>();
}

void pacman::EnterName::Init()
{
	m_TextCompPtr = GetOwner()->GetComponent<diji::Text>();
}

void pacman::EnterName::Update()
{
	if (m_IsDirty)
	{
		m_IsDirty = false;
		m_TextCompPtr->SetText(m_Name);
	}
}

void pacman::EnterName::MoveLetter(diji::Movement movement)
{
	switch (movement)
	{
	case diji::Movement::Up:
		m_Name[m_SelectedIndex] = m_Name[m_SelectedIndex] < 'Z' ? ++m_Name[m_SelectedIndex] : 'A';
		m_IsDirty = true;
		break;
	case diji::Movement::Down:
		m_Name[m_SelectedIndex] = m_Name[m_SelectedIndex] > 'A' ? --m_Name[m_SelectedIndex] : 'Z';
		m_IsDirty = true;
		break;
	case diji::Movement::Left:
		--m_SelectedIndex;
		if (m_SelectedIndex < 0)
			m_SelectedIndex = 2;
		break;
	case diji::Movement::Right:
		++m_SelectedIndex;
		m_SelectedIndex %= 3;
		break;
	case diji::Movement::Idle:
		ScoreBoard::GetInstance().AddScore(m_Name, m_ScoreCounterCompPtr->GetScore());
		diji::SceneManager::GetInstance().SetNextSceneToActivate(static_cast<int>(GameState::MENU));
		break;
	default:
		break;
	}
}
