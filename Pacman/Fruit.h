#pragma once
#include "Subject.h"
#include "Component.h"
#include "IObserver.h"

namespace diji
{
	enum class SoundId;
	class GameObject;
	class Collider;
}
namespace pacman
{
	class PelletObserver;
	class LevelObserver;

	class Fruit final : public diji::Component, public diji::Subject, public diji::IObserver
	{
	public:
		Fruit(diji::GameObject* ownerPtr, const diji::GameObject* player, const diji::GameObject* pelletCounter, const diji::GameObject* levelObserver);
		~Fruit() noexcept override = default;

		void Init() override;
		void Update() override;
		void FixedUpdate() override {};
		void OnNotify(diji::MessageTypes message, diji::Subject*) override;
		int GetValue() const { return m_Value; };
		void HandleCollision();

	private:
		int m_Value = 100;
		PelletObserver* m_PelletCounter;
		LevelObserver* m_LevelObserver;
		diji::Collider* m_PlayerColliderPtr;
		diji::Collider* m_OwnerColliderPtr;

		bool m_IsDisabled = true;
		bool m_IsEaten = false;
		bool m_ChangedFrame = false;
		float m_Timer = 0.0f;

		enum class FruitType
		{
			Cherry = 100,
			Strawberry = 300,
			Orange = 500,
			Apple = 700,
			Melon = 100,
			Galaxian = 2000,
			Bell = 3000,
			Key = 5000
		};

		const std::vector<FruitType> m_FruitCycle = 
		{ 
			FruitType::Cherry, 
			FruitType::Strawberry, 
			FruitType::Orange, 
			FruitType::Orange, 
			FruitType::Apple, 
			FruitType::Apple, 
			FruitType::Melon, 
			FruitType::Melon, 
			FruitType::Galaxian,
			FruitType::Galaxian,
			FruitType::Bell,
			FruitType::Bell,
			FruitType::Key };
	};
}

