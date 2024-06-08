#include "Music.h"
#include <SDL_mixer.h> 

diji::Music::Music(Mix_Music* music)
{
	m_MixMusicPtr = music;
}

diji::Music::~Music()
{
	Mix_FreeMusic(m_MixMusicPtr);
	m_MixMusicPtr = nullptr;
}

Mix_Music* diji::Music::GetMusic() const
{
	return m_MixMusicPtr;
}

void diji::Music::Play(bool repeat) const
{
	if (m_MixMusicPtr)
		Mix_PlayMusic(m_MixMusicPtr, repeat ? -1 : 0);
}

void diji::Music::SetVolume(const int volume) const
{
	volume == -1 ? Mix_VolumeMusic(16) : Mix_VolumeMusic(volume);
}

void diji::Music::Pause() const
{
	Mix_PauseMusic();
}

void diji::Music::Resume() const
{
	Mix_ResumeMusic();
}
