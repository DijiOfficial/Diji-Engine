#include "ISoundSystem.h"
#include "ResourceManager.h"
#include "SoundEffect.h"

namespace diji 
{
	std::unique_ptr<ISoundSystem> ServiceLocator::_ss_instance{ std::make_unique<NullSoundSystem>() };

	void SDLISoundSystem::PlaySound(const SoundId sound, const int volume) const
	{
        SoundEffect* soundEffect = nullptr;

        switch (sound)
        {
        case SoundId::PacmanDie:
            soundEffect = ResourceManager::GetInstance().LoadSoundEffect("pacman_death.wav");
            break;
        case SoundId::PacmanEatFruit:
            soundEffect = ResourceManager::GetInstance().LoadSoundEffect("pacman_eat_fruit.wav");
            break;
        case SoundId::PelletPickUp:
            soundEffect = ResourceManager::GetInstance().LoadSoundEffect("munch_1.wav");
            break;
        case SoundId::PelletPickUp2:
            soundEffect = ResourceManager::GetInstance().LoadSoundEffect("munch_2.wav");
            break;
        default:
            break;
        }

        if (soundEffect)
        {
            soundEffect->SetVolume(volume);
            soundEffect->Play();
        }
	}
}