#pragma once
#include "Singleton.h"
#include <map>
#include <string>

namespace pacman
{
	class ScoreBoard final : public diji::Singleton<ScoreBoard>
	{
	public:
		void Init();
		void AddScore(const std::string& name, int score);
		int GetHighestScore() const;
		const std::multimap<int, std::string, std::greater<int>>& GetScoreboard() const { return m_ScoreboardMap; }
	
	private:
		std::multimap<int, std::string, std::greater<int>> m_ScoreboardMap;
	};
}
