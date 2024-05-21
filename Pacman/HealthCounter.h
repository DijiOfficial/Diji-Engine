#pragma once
#include "Subject.h"
#include "Component.h"

namespace pacman
{
	class HealthCounter final : public diji::Component, public diji::Subject
	{
	public:
		HealthCounter(diji::GameObject* ownerPtr, int health);
		~HealthCounter() override = default;

		HealthCounter(const HealthCounter& other) = delete;
		HealthCounter(HealthCounter&& other) = delete;
		HealthCounter& operator=(const HealthCounter& other) = delete;
		HealthCounter& operator=(HealthCounter&& other) = delete;

		void Update() override {};
		void FixedUpdate() override {};
		void DecreaseHealth();

		int GetHealth() const { return m_Health; }

	private:	
		int m_Health;
	};
}

