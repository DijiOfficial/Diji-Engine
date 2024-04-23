//#include "Music.h"
//#pragma warning(push)
//#pragma warning(disable:4099)
//#include <SDL_mixer.h> 
//#pragma warning(pop)
//
//diji::Music::Music(Mix_Music* sound)
//{
//	m_MusicPtr = sound;
//}
//
//diji::Music::~Music()
//{
//	Mix_FreeMusic(m_MusicPtr);
//	m_MusicPtr = nullptr;
//}
//
//Mix_Music* diji::Music::GetMusic() const
//{
//	return m_MusicPtr;
//}
//
//void diji::Music::Play(bool repeat) const
//{
//	Mix_PlayMusic(m_MusicPtr, repeat ? -1 : 1);
//}
//
//void diji::Music::SetVolume(const int volume) const
//{
//	volume == -1 ? Mix_VolumeMusic(64) : Mix_VolumeMusic(volume);
//}
