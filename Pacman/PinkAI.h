#pragma once
#include "Command.h"

namespace diji
{
	// ┌─────────┐Complex Algortihm┌────────────┐ Is inside Maze ┌───────────┐      
	// │ Waiting ├─────────────────► Enter Maze ├────────────────► Chase Pac │      
	// └─────────┘                 └────▲───────┘                └─────┬─────┘      
	//                                  │                              │            
	//                                  │                              │   If Killed
	//                                  │                              │            
	//                                  │                        ┌─────▼─────┐      
	//                                  │                        │ Return To │      
	//                                  └────────────────────────┤   Spawn   │      
	//                                      If Back To Spawn     └───────────┘      
	class Texture;
	class Transform;
	class Collider;

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

		virtual std::unique_ptr<GhostState> Execute(Transform* transform, Collider* collider, Collider* player) = 0;
	};

	class PinkAI final : public Component
	{
	public:
		PinkAI(GameObject* ownerPtr, GameObject* player);
		~PinkAI() = default;

		PinkAI(const PinkAI& other) = delete;
		PinkAI(PinkAI&& other) = delete;
		PinkAI& operator=(const PinkAI& other) = delete;
		PinkAI& operator=(PinkAI&& other) = delete;

		void Update() override;

	private:
		Transform* m_TransformCompPtr;
		Collider* m_ColliderCompPtr;
		Collider* m_PlayerColliderPtr;

		std::unique_ptr<GhostState> m_CurrentStateUPtr;
	};

	class Waiting final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Waiting() override = default;

		std::unique_ptr<GhostState> Execute(Transform* transform, Collider* collider, Collider* player) override;

	private:
		float tempTimer = 0.0f;
	};

	class EnterMaze final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~EnterMaze() override = default;

		std::unique_ptr<GhostState> Execute(Transform* transform, Collider* collider, Collider* player) override;
	private:
		bool tempLock = false;
	};

	class ChasePac final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~ChasePac() override = default;

		std::unique_ptr<GhostState> Execute(Transform* transform, Collider* collider, Collider* player) override;
	};

	class ReturnToSpawn final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~ReturnToSpawn() override = default;

		std::unique_ptr<GhostState> Execute(Transform* transform, Collider* collider, Collider* player) override;
	};
}

