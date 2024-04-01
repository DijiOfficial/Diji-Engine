//#pragma once
//
//#include "SteamAchievements.h"
//#include "Singleton.h"
//#include <memory>
//
//namespace diji 
//{
//	class CSteamAchievements;
//	enum EAchievements
//	{
//		ACH_WIN_ONE_GAME = 0
//		//ACH_WIN_100_GAMES = 1,
//		//ACH_TRAVEL_FAR_ACCUM = 2,
//		//ACH_TRAVEL_FAR_SINGLE = 3,
//	};
//
//	class Achievements : public Singleton<Achievements>
//	{
//	public:
//		Achievements() = default;
//
//		void SetAchievement(const char* ID);
//
//		void Init();
//	private:
//		Achievement_t m_Achievements[1] =
//		{
//			_ACH_ID(ACH_WIN_ONE_GAME, "Winner")
//			//_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
//			//_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
//			//_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter"),
//		};
//
//		std::unique_ptr<CSteamAchievements> m_SteamAchievementsUPtr;
//	};
//}
//
//
//
