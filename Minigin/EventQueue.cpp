#include "EventQueue.h"

void diji::SoundEventQueue::AddSoundRequest(SoundId sound, int volume)
{
	std::lock_guard<std::mutex> lock(soundMutex_);
	if (not m_SoundQueue.empty())
	{
		auto pendingSound = m_SoundQueue.front();
		if (sound == pendingSound.first)
			return;
	}
	
	m_SoundQueue.push({ sound, volume });
	condition_.notify_one();
}

std::pair<diji::SoundId, int> diji::SoundEventQueue::GetNextSoundRequest()
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

void diji::SoundProcessor::Start()
{
	if (!m_IsRunning)
	{
		m_IsRunning = true;
		SoundEventQueue::GetInstance().Start();
		m_SoundThread = std::jthread(&SoundProcessor::ProcessSounds, this);
	}
}

void diji::SoundProcessor::Stop()
{
	if (m_IsRunning)
	{
		m_IsRunning = false;
		SoundEventQueue::GetInstance().Stop();
	}
}

void diji::SoundProcessor::ProcessSounds() const
{
	auto& soundSystem = ServiceLocator::GetSoundSystem();
	auto& SoundEventQueue = SoundEventQueue::GetInstance();

	while (m_IsRunning)
	{
		auto request = SoundEventQueue.GetNextSoundRequest();
		soundSystem.PlaySound(request.first, request.second);
	}
}