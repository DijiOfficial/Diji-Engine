#pragma once

class Mix_Chunk;
namespace diji 
{
	class SoundEffect final
	{
	public:
		explicit SoundEffect(Mix_Chunk* sound);
		~SoundEffect();

		SoundEffect(const SoundEffect&) = delete;
		SoundEffect(SoundEffect&&) = delete;
		SoundEffect& operator= (const SoundEffect&) = delete;
		SoundEffect& operator= (const SoundEffect&&) = delete;

		Mix_Chunk* GetSoundEffect() const;
		void Play() const;
		void SetVolume(const int volume) const;

	private:
		Mix_Chunk* m_MixChunkPtr;
	};
}

