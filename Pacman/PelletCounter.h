#pragma once
#include "Subject.h"
#include "Component.h"

namespace pacman
{
	class PelletObserver;

	class PelletCounter final : public diji::Component, public diji::Subject
	{
	public:
		PelletCounter(diji::GameObject* ownerPtr) : Component(ownerPtr) {};
		~PelletCounter() noexcept override = default;

		void Init() override;
		void Update() override;
		void FixedUpdate() override {};

		void test();
	private:
		PelletObserver* m_PelletObserverPtr = nullptr;
		bool m_CanNotify = true;
		bool m_IsActive = false;
		int m_PelletCountAtNotification = 0;
	};
}

