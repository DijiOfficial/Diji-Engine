#pragma once
#include "Component.h"
#include <map>
#include <string>
#include <vector>
#include "IObserver.h"

namespace pacman
{
	class Intro final : public diji::Component, public diji::IObserver
	{
	public:
		Intro(diji::GameObject* ownerPtr);
		~Intro() noexcept override = default;

		void Init() override {};
		void Update() override {};
		void FixedUpdate() override {};
		void LateUpdate() override;
		void OnNotify(diji::MessageTypes message, diji::Subject*) override;

		std::vector<std::pair<std::string, std::string>> GetGhostNames() const { return m_GhostNames.at(m_CurrentIntroCycle); };

	private:
		std::map<int, std::vector<std::pair<std::string, std::string>>> m_GhostNames;
		int m_CurrentIntroCycle = 0;
		bool m_ResetMenu = false;
	};
}