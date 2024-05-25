#pragma once
struct _Mix_Music;
typedef struct _Mix_Music Mix_Music;

namespace diji
{
	class Music final
	{
	public:
		explicit Music(Mix_Music* music);
		~Music() noexcept;

		Music(const Music&) = delete;
		Music(Music&&) = delete;
		Music& operator= (const Music&) = delete;
		Music& operator= (const Music&&) = delete;

		Mix_Music* GetMusic() const;
		void Play(bool repeat) const;
		void SetVolume(const int volume) const;

	private:
		Mix_Music* m_MixMusicPtr;
	};
}

