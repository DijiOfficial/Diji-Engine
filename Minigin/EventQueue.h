#pragma once
#include "Singleton.h"
#include "ISoundSystem.h"
#include <queue>
#include <mutex>

namespace diji 
{
    class SoundEventQueue final : public Singleton<SoundEventQueue>
    {
    public:
        void AddSoundRequest(SoundId sound, int volume);
        std::pair<SoundId, int> GetNextSoundRequest();

        void Stop() { m_IsRunning = false; condition_.notify_one(); }
        void Start() { m_IsRunning = true; }

        //originaly in PickUpManager but PickUp doesnt have access
        void SwitchPickUpState() { m_FirstPickUp = !m_FirstPickUp; };
        bool IsFirstPickUp() const { return m_FirstPickUp; };
    private:
        std::queue<std::pair<SoundId, int>> m_SoundQueue;
        std::mutex soundMutex_;
        bool m_IsRunning = false;
        bool m_FirstPickUp = true;

        // Allows threads to synchronize their execution based on certain conditions. 
        // It provides a way for one thread to notify other threads when a particular condition becomes true, allowing them to proceed with their execution
        std::condition_variable condition_;
    };

    class SoundProcessor final
    {
    public:
        SoundProcessor() { Start(); };
        ~SoundProcessor() { Stop(); };

    private:
        void ProcessSounds() const;
        void Start();
        void Stop();

        std::jthread m_SoundThread;
        bool m_IsRunning = false;
    };
}

