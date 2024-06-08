#include "ISoundSystem.h"
#include "ResourceManager.h"
#include "SoundEffect.h"
#include "Music.h"

#include <iostream>

namespace diji 
{
	std::unique_ptr<ISoundSystem> ServiceLocator::_ss_instance{ std::make_unique<NullSoundSystem>() };

	void SDLISoundSystem::PlaySound(const std::string& audio, bool isMusic, const int volume) const
	{
        SoundEffect* soundEffect = nullptr;
		Music* music = nullptr;

		if (isMusic)
			music = ResourceManager::GetInstance().LoadMusic(audio);
		else
			soundEffect = ResourceManager::GetInstance().LoadSoundEffect(audio);

        if (soundEffect)
        {
            soundEffect->SetVolume(volume);
            soundEffect->Play();
        }

		if (music)
		{
			music->Play(true);
			music->SetVolume(volume);
		}
	}

	SDLISoundSystem::SDLISoundSystem()
	{
		if (!m_IsRunning)
		{
			m_IsRunning = true;
			m_SoundThread = std::jthread([this]() { ProcessSounds(); });
		}
	}

	SDLISoundSystem::~SDLISoundSystem()
	{
		if (m_IsRunning)
		{
			m_IsRunning = false;
			condition_.notify_one();
		}
	}

	void SDLISoundSystem::AddSoundRequest(const std::string& audio, bool isMusic, int volume)
	{
		std::lock_guard<std::mutex> lock(soundMutex_);
		if (not m_SoundQueue.empty())
		{
			auto pendingSound = m_SoundQueue.front();
			if (audio == pendingSound.second) // check volume and set the highest volume
				return;
		}

		m_SoundQueue.push({ { isMusic, volume }, audio });
		condition_.notify_one();
	}

	std::pair<std::pair<bool, int>, std::string> SDLISoundSystem::GetNextSoundRequest()
	{
		std::unique_lock<std::mutex> lock(soundMutex_);

		// Block the request until a new sound request is added or the thread is stopped
		condition_.wait(lock, [this] { return !m_SoundQueue.empty() || not m_IsRunning; });

		if (!m_IsRunning)
			return { { false, 0 }, "invalid"};

		auto request = m_SoundQueue.front();
		m_SoundQueue.pop();
		return request;
	}

	void SDLISoundSystem::ProcessSounds() 	{
		while (m_IsRunning)
		{
			auto request = GetNextSoundRequest();
			PlaySound(request.second, request.first.first, request.first.second);
		}
	}

	void NullSoundSystem::AddSoundRequest(const std::string& , bool , int)
	{
		std::cout << "No Sound System available\n";
	}

	void LoggingSoundSystem::AddSoundRequest(const std::string& audio, bool isMusic, int volume)
	{
		_real_ss->AddSoundRequest(audio, isMusic, volume);
		std::cout << "Adding sound request " << audio << " at volume " << volume << std::endl;
	}
}