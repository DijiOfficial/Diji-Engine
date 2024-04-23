#pragma once
#include <memory>
#include <iostream>
#include "SoundEffect.h"

namespace diji 
{
	enum class SoundId
	{
		PacmanDie,
		PacmanEatFruit,
		test
	};

	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;
		virtual void PlaySound(const SoundId sound, const int volume = -1) const = 0;
	};

	class NullSoundSystem final : public ISoundSystem
	{
	public:
		void PlaySound(const SoundId, const int) const override {};
	};

	class ServiceLocator final
	{
	public:
		static ISoundSystem& GetSoundSystem() { return *_ss_instance; };
		static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& ss) 
		{ 
			_ss_instance = ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss);
		};

	private:
		static std::unique_ptr<ISoundSystem> _ss_instance;
	};

	class SDLISoundSystem final : public ISoundSystem
	{
	public:
		virtual void PlaySound(const SoundId sound, const int volume) const override;
	};

	class LoggingSoundSystem final : public ISoundSystem
	{
	public:
		explicit LoggingSoundSystem(std::unique_ptr<ISoundSystem>&& ss) : _real_ss{ std::move(ss) } {};
		virtual ~LoggingSoundSystem() override = default;

		void PlaySound(const SoundId id, const int volume) const override
		{
			_real_ss->PlaySound(id, volume);
			std::cout << "Playing sound " << static_cast<int>(id) << " at volume " << volume << std::endl;
		};

	private:
		std::unique_ptr<ISoundSystem> _real_ss;
	};
}