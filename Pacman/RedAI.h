#pragma once
#include "GhostAI.h"
namespace diji
{
	//                                                            ┌─────────┐
	//															  │ Scatter │
	//															  └───▲─────┘  
	//                                                                │       If Pac has               
	// ┌─────────┐Complex Algortihm┌────────────┐ Is inside Maze ┌────▲──────┐ power up┌──────────┐    
	// │ Waiting ├─────────────────► Enter Maze ├────────────────► Chase Pac ◄─────────► Flee Pac │    
	// └─────────┘                 └────▲───────┘                └───────────┘         └────┬─────┘    
	//                                  │                                                   │          
	//                                  │                                                   │          
	//                                  │                                                   │ If Killed
	//                                  │                        ┌───────────┐              │          
	//                                  │                        │ Return To │              │          
	//                                  └────────────────────────┤   Spawn   ◄──────────────┘          
	//                                                           └───────────┘                         
	class Collider;
	class Transform;

	class RedAI final : public GhostAI
	{
	public:
		RedAI(GameObject* ownerPtr, GameObject* player);
		~RedAI() = default;

		void Update() override {};
	};

	class RedChase final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~RedChase() override = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(Transform* transform, Collider* collider, Collider* player) override;
	};

	//class ReturnToSpawn final : public GhostState
	//{
	//public:
	//	using GhostState::GhostState;
	//	~ReturnToSpawn() override = default;

	//	std::unique_ptr<GhostState> Execute(Transform* transform, Collider* collider, Collider* player) override;
	//};

	//class EnterMaze final : public GhostState
	//{
	//public:
	//	using GhostState::GhostState;
	//	~EnterMaze() override = default;

	//	void OnEnter() override {};
	//	void OnExit() override {};
	//	std::unique_ptr<GhostState> Execute(Transform* transform, Collider* collider, Collider* player) override;
	//private:
	//	bool tempLock = false;
	//};
}

