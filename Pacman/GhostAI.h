#pragma once
#include "Component.h"
#include "ChaseScatterAlgo.h"
#include "IObserver.h"
#include "GhostStates.h"

#include <string>

namespace diji
{
	class Transform;
	class Collider;
	enum class Movement;
	class Texture;
	struct Rectf;
}

namespace pacman
{ 
	//todo: add the dying state
	//                                                                      If Power                
	//                                    If In Chase Mode    ┌───────┐   Pellet Eaten              
	//                               ┌────────────────────────► Chase ◄──────────────────┐          
	//                               │                        └───▲───┘   Power Ran Out  │          
	//                               │                            │                      │          
	//                               │                            │                      │          
	//                               │                          If│Time            ┌─────▼──────┐   
	//                               │                          To│Switch          │ Frightened ├──┐
	//                               │                            │                └─────▲──────┘  │
	//                               │                            │                      │         │
	//               Complex         │            If In           │         If Power     │         │
	// ┌─────────┐  Algorithm  ┌─────┴──────┐  Scatter Mode  ┌────▼────┐  Pellet Eaten   │         │
	// │ Waiting ├─────────────► Enter Maze ├────────────────► Scatter ◄─────────────────┘         │
	// └─────────┘             └─────▲──────┘                └─────────┘  Power Ran Out            │
	//                               │                                                             │
	//                      Personnal│Spawn                                                        │
	//                         Is Reached                                                          │
	//                               │                                                             │
	//                          ┌────┴────┐  If Spawn Reached  ┌───────┐    If Player Collision    │
	//                          │ Respawn ◄────────────────────┤ Eaten ◄───────────────────────────┘
	//                          └─────────┘                    └───────┘                            
	
	class PelletObserver;
	//todo: add points whgen eating ghosts(probably better when all 4ghosts are there)
	//todo: test audio cases with multiple ghosts
	//todo: ghosts can be eaten when coming out the maze?

	class GhostAI : public diji::Component, public diji::IObserver
	{
	public:
		~GhostAI() noexcept = default;

		GhostAI(const GhostAI& other) = delete;
		GhostAI(GhostAI&& other) = delete;
		GhostAI& operator=(const GhostAI& other) = delete;
		GhostAI& operator=(GhostAI&& other) = delete;

		void Init() override;
		void Update() override;
		void FixedUpdate() override;
		void OnNotify(diji::MessageTypes message, diji::Subject* subject) override;

		virtual std::unique_ptr<GhostState> GetChaseState() const = 0;

		diji::Transform* GetTransform() const { return m_TransformCompPtr; };
		diji::Collider* GetCollider() const { return m_ColliderCompPtr; };
		diji::Collider* GetPlayerCollider() const { return m_PlayerColliderPtr; };
		diji::Texture* GetTexture() const { return m_TextureCompPtr; };
		bool GetIsInChaseState() const { return m_ChaseScatterAlgo->IsInChaseState(); };
		glm::vec2 GetSpawnPoint() const { return m_PersonnalSpawn; };
		glm::vec2 GetScatterTarget() const { return m_ScatterTarget; };
		GhostState* GetCurrentState() const { return m_CurrentStateUPtr.get(); };
		int GetPelletCount() const;

		bool IsFrightened() const { return m_IsFrightened; };
		bool IsPowerAlmostOver() const { return m_PowerUpTimer >= 7.f; };
		void ClearFrightened() const;
		void SetGhostTexture() const;
		bool IsUpdatePaused() const { return m_UpdateIsPaused; };
		void TurnAround() const;
	protected:
		GhostAI(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter);

		std::unique_ptr<GhostState> m_CurrentStateUPtr = nullptr;
		glm::vec2 m_PersonnalSpawn = { 0, 0 };
		glm::vec2 m_ScatterTarget = { 0, 0 };
		std::string m_TexturePath = "";

	private:
		diji::Collider* m_PlayerColliderPtr;
		const pacman::PelletObserver* m_PelletCounterPtr;
		diji::Collider* m_ColliderCompPtr;
		diji::Transform* m_TransformCompPtr;
		diji::Texture* m_TextureCompPtr;
		std::unique_ptr<ChaseScatterAlgo> m_ChaseScatterAlgo = std::make_unique<ChaseScatterAlgo>();
		mutable float m_PowerUpTimer = 0.f;
		mutable bool m_IsFrightened = false;
		bool m_UpdateIsPaused = false;
		float m_PausedTimer = 0.f;
		const float m_TunnelSpeed = 0.9375f;
	};

	class RedAI final : public GhostAI
	{
	public:
		RedAI(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter);
		~RedAI() noexcept = default;

		std::unique_ptr<GhostState> GetChaseState() const override;

		void Init() override;
	};

	class Pinky final : public GhostAI
	{
	public:
		Pinky(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter);
		~Pinky() noexcept = default;

		std::unique_ptr<GhostState> GetChaseState() const override;

		void Init() override;
	};

	class Inky final : public GhostAI
	{
	public:
		Inky(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter);
		~Inky() noexcept = default;

		std::unique_ptr<GhostState> GetChaseState() const override;

		void Init() override;
	};

	class Clyde final : public GhostAI
	{
	public:
		Clyde(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter);
		~Clyde() noexcept = default;

		std::unique_ptr<GhostState> GetChaseState() const override;

		void Init() override;
	};

}

