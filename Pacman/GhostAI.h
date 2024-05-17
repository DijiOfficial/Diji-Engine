#pragma once
#include "Component.h"
namespace diji
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
	class Transform;
	class Collider;

	class GhostAI;
	class GhostState
	{
	public:
		//todo: set destructors to noexcept
		//GhostState(Transform* transform, Collider* collider, Collider* player);
		GhostState() = default;
		virtual ~GhostState() = default;

		GhostState(const GhostState& other) = delete;
		GhostState(GhostState&& other) = delete;
		GhostState& operator=(const GhostState& other) = delete;
		GhostState& operator=(GhostState&& other) = delete;

		virtual void OnEnter([[maybe_unused]] const GhostAI* ghost) = 0;
		virtual void OnExit([[maybe_unused]] const GhostAI* ghost) = 0;
		virtual std::unique_ptr<GhostState> Execute(Transform* transform, Collider* collider, Collider* player) = 0;
	};

	class GhostAI : public Component
	{
	public:
		GhostAI(GameObject* ownerPtr, GameObject* player);
		~GhostAI() = default;

		GhostAI(const GhostAI& other) = delete;
		GhostAI(GhostAI&& other) = delete;
		GhostAI& operator=(const GhostAI& other) = delete;
		GhostAI& operator=(GhostAI&& other) = delete;

		void Update() override = 0;
		void FixedUpdate() override;

		Transform* GetTransform() const { return m_TransformCompPtr; };
		Collider* GetCollider() const { return m_ColliderCompPtr; };
		Collider* GetPlayerCollider() const { return m_PlayerColliderPtr; };

	protected:
		Transform* m_TransformCompPtr;
		Collider* m_ColliderCompPtr;
		Collider* m_PlayerColliderPtr;

		std::unique_ptr<GhostState> m_CurrentStateUPtr;
	};


	
}

