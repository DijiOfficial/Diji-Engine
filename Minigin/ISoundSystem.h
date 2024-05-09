#pragma once
#include <memory>
#include <iostream>
#include <queue>
#include <mutex>
namespace diji 
{
	enum class SoundId
	{
		InvalidSoundId = -1,
		None, 
		PacmanDie,
		PacmanEatFruit,
		PelletPickUp,
		PelletPickUp2,
	};

	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;
		virtual void AddSoundRequest(SoundId sound, int volume) = 0;
	};

	class NullSoundSystem final : public ISoundSystem
	{
	public:
		void AddSoundRequest(SoundId sound, int volume) override { (void)sound; (void)volume; std::cout << "No Sound System available\n"; };
	};

	class ServiceLocator final
	{
	public:
		static ISoundSystem& GetSoundSystem() { return *_ss_instance; };
		static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& ss) 
		{ 
			_ss_instance = ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss); //assign null system instead or nothing since its alread null osund
		};

	private:
		static std::unique_ptr<ISoundSystem> _ss_instance;
	};

	class SDLISoundSystem final : public ISoundSystem
	{		
	public:
		SDLISoundSystem() { Start(); };
		~SDLISoundSystem() { Stop(); };
		void AddSoundRequest(SoundId sound, int volume) override;
		
	private:
		void PlaySound(const SoundId sound, const int volume) const;
		std::pair<SoundId, int> GetNextSoundRequest();
		
		void Stop();
		void Start();
		void ProcessSounds();

		std::jthread m_SoundThread;
		std::queue<std::pair<SoundId, int>> m_SoundQueue;
		std::mutex soundMutex_;
		bool m_IsRunning = false;
		// Allows threads to synchronize their execution based on certain conditions. 
		// It provides a way for one thread to notify other threads when a particular condition becomes true, allowing them to proceed with their execution
		std::condition_variable condition_;
	};

	class LoggingSoundSystem final : public ISoundSystem
	{
	public:
		explicit LoggingSoundSystem(std::unique_ptr<ISoundSystem>&& ss) : _real_ss{ std::move(ss) } {};
		virtual ~LoggingSoundSystem() override = default;

		void AddSoundRequest(SoundId sound, int volume) override 
		{ 
			_real_ss->AddSoundRequest(sound, volume);
			std::cout << "Adding sound request " << static_cast<int>(sound) << " at volume " << volume << std::endl;
		};
	private:
		std::unique_ptr<ISoundSystem> _real_ss;
	};
}