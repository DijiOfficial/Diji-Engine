#include "ISoundSystem.h"

namespace diji 
{
	std::unique_ptr<ISoundSystem> ServiceLocator::_ss_instance{ std::make_unique<NullSoundSystem>() };

	void SDLISoundSystem::PlaySound(const SoundId sound, const int volume) const
	{
		(void)sound;
		(void)volume;
	}
}