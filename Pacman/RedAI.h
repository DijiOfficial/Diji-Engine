#pragma once
#include "GhostAI.h"

namespace pacman
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
	
	class RedAI final : public GhostAI
	{
	public:
		RedAI(diji::GameObject* ownerPtr, diji::GameObject* player);
		~RedAI() = default;

		std::unique_ptr<GhostState> GetChaseState() const override { return std::make_unique<RedChase>(); };

		void Init() override;
		//void Update() override { GhostAI::Update(); };
	};
}

