#pragma once
#include "IObserver.h"
#include "Text.h"
#include "Component.h"

namespace diji
{
	class Texture;
}

namespace pacman 
{
	enum class MessageTypesDerived
	{
		HEALTH_CHANGE,
		SCORE_CHANGE,
		LEVEL_COLLISION,
		ENEMY_COLLISION,
		PICKUP_COLLISION,
		POWERUP_COLLISION,
	};

	class HealthObserver final : public diji::Text, public diji::IObserver
	{
	public:
		using Text::Text;
		~HealthObserver() noexcept override = default;

		void OnNotify(diji::MessageTypes message, diji::Subject* subject) override;
	};

	class PacmanHealthObserver final : public diji::Component, public diji::IObserver
	{
	public:
		PacmanHealthObserver(diji::GameObject* ownerPtr);
		~PacmanHealthObserver() noexcept override = default;

		virtual void Init();
		virtual void Update() {};
		virtual void FixedUpdate() {};

		void OnNotify(diji::MessageTypes message, diji::Subject* subject) override;
	
	private:
		diji::Texture* m_TextureCompPtr = nullptr;
		const int M_LIFE_TEXTURE_WIDTH = 16;
	};

	class ScoreObserver final : public diji::Text, public diji::IObserver
	{
	public:
		using Text::Text;
		~ScoreObserver() noexcept override = default;

		void OnNotify(diji::MessageTypes message, diji::Subject* subject) override;
	};

	class PelletObserver final : public diji::Component, public diji::IObserver
	{
	public:
		PelletObserver(diji::GameObject* ownerPtr) : Component(ownerPtr) {};
		~PelletObserver() noexcept override = default;

		void OnNotify(diji::MessageTypes message, diji::Subject* subject) override;

		void Init() override {};
		void Update() override {};
		void FixedUpdate() override {};
		int GetPelletCount() const { return m_PelletCount; }
	
	private:
		int m_PelletCount = 0;
	};
}

