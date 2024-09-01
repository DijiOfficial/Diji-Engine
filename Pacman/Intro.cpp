#include "Intro.h"
#include "Observers.h"
#include "CustomTextRender.h"
#include "GameObject.h"
#include "GameLoader.h"

pacman::Intro::Intro(diji::GameObject* ownerPtr)
	: diji::Component(ownerPtr)
{
	m_GhostNames[0].push_back(std::make_pair("OIKAKE----", "\"AKABEI\""));
	m_GhostNames[0].push_back(std::make_pair("MACHIBUSE--", "\"PINKY\""));
	m_GhostNames[0].push_back(std::make_pair("KIMAGURE--", "\"AOSUKE\""));
	m_GhostNames[0].push_back(std::make_pair("OTOBOKE---", "\"GUZUTA\""));

	m_GhostNames[2].push_back(std::make_pair("URCHIN-----", "\"MACKY\""));
	m_GhostNames[2].push_back(std::make_pair("ROMP-------", "\"MICKY\""));
	m_GhostNames[2].push_back(std::make_pair("STYLIST----", "\"MUCKY\""));
	m_GhostNames[2].push_back(std::make_pair("CRYBABY----", "\"MOCKY\""));

	m_GhostNames[1].push_back(std::make_pair("-SHADOW   ", "\"BLINKY\""));
	m_GhostNames[1].push_back(std::make_pair("-SPEEDY   ", "\"PINKY\""));
	m_GhostNames[1].push_back(std::make_pair("-BASHFUL  ", "\"INKY\""));
	m_GhostNames[1].push_back(std::make_pair("-POKEY    ", "\"CLYDE\""));

	m_GhostNames[3].push_back(std::make_pair("-AAAAAAA- ", "\"BBBBBB\""));
	m_GhostNames[3].push_back(std::make_pair("-CCCCCCC- ", "\"DDDDDD\""));
	m_GhostNames[3].push_back(std::make_pair("-EEEEEEE- ", "\"FFFFFF\""));
	m_GhostNames[3].push_back(std::make_pair("-GGGGGGG- ", "\"HHHHHH\""));
}

void pacman::Intro::LateUpdate()
{
	if (m_ResetMenu)
	{
		Loader::PacmanMenuReload();
		m_ResetMenu = false;
	}
}

void pacman::Intro::OnNotify(diji::MessageTypes message, diji::Subject*)
{
	auto msg = static_cast<MessageTypesDerived>(message);
	if (msg == MessageTypesDerived::RESET_MENU)
	{
		m_CurrentIntroCycle = ++m_CurrentIntroCycle % 4;
		GetOwner()->GetComponent<pacman::IntroRender>()->Reset();
		m_ResetMenu = true;
	}
}
