#include "LevelIntro.h"
#include "TimeSingleton.h"
#include "Observers.h"
#include "ISoundSystem.h"

void pacman::LevelIntro::Update()
{
	if (m_StartLevel)
		return;

	if (not m_MusicStarted)
	{
		diji::ServiceLocator::GetSoundSystem().AddSoundRequest(diji::SoundId::GameStart, -1);
		m_MusicStarted = true;
	}

	m_Timer += diji::TimeSingleton::GetInstance().GetDeltaTime();

	if (m_Timer >= 2.f and m_SpawnNotify)
	{
		Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::LEVEL_BEGIN));
		m_SpawnNotify = false;
	}

	if (m_Timer >= 4.2f)
	{
		diji::ServiceLocator::GetSoundSystem().AddSoundRequest(diji::SoundId::Music, -1);
		Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::LEVEL_START));
		m_StartLevel = true;
	}
}
