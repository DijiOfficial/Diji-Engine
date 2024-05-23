#pragma once
#include "GhostAI.h"
#include "GhostStates.h"
namespace pacman
{
	//                                                                        If Pac has               
	// ┌─────────┐Complex Algortihm┌────────────┐ Is inside Maze ┌───────────┐ power up┌──────────┐    
	// │ Waiting ├─────────────────► Enter Maze ├────────────────► Chase Pac ◄─────────► Flee Pac │    
	// └─────────┘                 └────▲───────┘                └───────────┘         └────┬─────┘    
	//                                  │                                                   │          
	//                                  │                                                   │          
	//                                  │                                                   │ If Killed
	//                                  │                        ┌───────────┐              │          
	//                                  │                        │ Return To │              │          
	//                                  └────────────────────────┤   Spawn   ◄──────────────┘          
	//                                                           └───────────┘                         

	class PinkAI final : public GhostAI
	{
	public:
		PinkAI(diji::GameObject* ownerPtr, diji::GameObject* player);

		void FixedUpdate() override;
	};

	class Waiting final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Waiting() override = default;

		void OnEnter(const GhostAI*) override {};
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;

	private:
		float tempTimer = 0.0f;
	};

	class ChasePac final : public GhostState
	{
	public:
		using GhostState::GhostState;

		void OnEnter(const GhostAI*) override {};
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	};

	class ReturnToSpawn final : public GhostState
	{
	public:
		using GhostState::GhostState;

		void OnEnter(const GhostAI*) override {};
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	};

	class EnterMaze final : public GhostState
	{
	public:
		using GhostState::GhostState;

		void OnEnter(const GhostAI*) override {};
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		bool tempLock = false;
	};
}

