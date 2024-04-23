#include "SoundEffect.h"
#include <SDL_mixer.h> 

diji::SoundEffect::SoundEffect(Mix_Chunk* sound)
{
	m_MixChunkPtr = sound;
}

diji::SoundEffect::~SoundEffect()
{
	Mix_FreeChunk(m_MixChunkPtr);
	m_MixChunkPtr = nullptr;
}

Mix_Chunk* diji::SoundEffect::GetSoundEffect() const
{
	return m_MixChunkPtr;
}

void diji::SoundEffect::Play() const
{
	Mix_PlayChannel(-1, m_MixChunkPtr, 0);
}

void diji::SoundEffect::SetVolume(const int volume) const
{
	volume == -1 ? Mix_VolumeChunk(m_MixChunkPtr, 64) : Mix_VolumeChunk(m_MixChunkPtr, volume);
}
