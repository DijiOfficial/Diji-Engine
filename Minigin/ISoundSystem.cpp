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
		case SoundId::PowerPellet:
			soundEffect = ResourceManager::GetInstance().LoadSoundEffect("power_pellet.wav");
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


	void SDLISoundSystem::AddSoundRequest(SoundId sound, int volume)
	{
		std::lock_guard<std::mutex> lock(soundMutex_);
		if (not m_SoundQueue.empty())
		{
			auto pendingSound = m_SoundQueue.front();
			if (sound == pendingSound.first) // check volume and set the highest volume
				return;
		}

		m_SoundQueue.push({ sound, volume });
		condition_.notify_one();
	}

	std::pair<SoundId, int> SDLISoundSystem::GetNextSoundRequest()
	{
		std::unique_lock<std::mutex> lock(soundMutex_);

		// Block the request until a new sound request is added or the thread is stopped
		condition_.wait(lock, [this] { return !m_SoundQueue.empty() || not m_IsRunning; });

		if (!m_IsRunning)
			return { SoundId::InvalidSoundId, 0 };

		auto request = m_SoundQueue.front();
		m_SoundQueue.pop();
		return request;
	}

	void SDLISoundSystem::Start()
	{
		if (!m_IsRunning)
		{
			m_IsRunning = true;
			m_SoundThread = std::jthread([this]() { ProcessSounds(); });
		}
	}

	void SDLISoundSystem::Stop()
	{
		if (m_IsRunning)
		{
			m_IsRunning = false;
			condition_.notify_one();
		}
	}

	void SDLISoundSystem::ProcessSounds() 	{
		while (m_IsRunning)
		{
			auto request = GetNextSoundRequest();
			PlaySound(request.first, request.second);
		}
	}
}