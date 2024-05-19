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
	
	
	//class Collider;
	//class Transform;
	//enum class Movement;
	class RedAI final : public GhostAI
	{
	public:
		RedAI(GameObject* ownerPtr, GameObject* player);
		~RedAI() = default;

		std::unique_ptr<GhostState> GetChaseState() const override { return std::make_unique<RedChase>(); };

		//void Update() override { GhostAI::Update(); };
	};

	//class RedChase final : public GhostState
	//{
	//public:
	//	using GhostState::GhostState;
	//	~RedChase() override = default;

	//	void OnEnter(const GhostAI* ghost) override;
	//	void OnExit(const GhostAI*) override {};
	//	std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	//};
}

