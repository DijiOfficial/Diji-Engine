#pragma once
#include "Component.h"
#include "ChaseScatterAlgo.h"
#include "IObserver.h"

#include <glm/vec2.hpp>
#include <memory>
#include <string>

namespace diji
{
	class Transform;
	class Collider;
	enum class Movement;
	class Texture;
}

namespace pacman
{
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
	
	class GhostState;

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
		void OnNotify(diji::MessageTypes message, diji::Subject*) override;

		virtual std::unique_ptr<GhostState> GetChaseState() const = 0;

		diji::Transform* GetTransform() const { return m_TransformCompPtr; };
		diji::Collider* GetCollider() const { return m_ColliderCompPtr; };
		diji::Collider* GetPlayerCollider() const { return m_PlayerColliderPtr; };
		diji::Texture* GetTexture() const { return m_TextureCompPtr; };
		bool GetIsInChaseState() const { return m_ChaseScatterAlgo->IsInChaseState(); };
		glm::vec2 GetSpawnPoint() const { return m_PersonnalSpawn; };
		glm::vec2 GetScatterTarget() const { return m_ScatterTarget; };

		bool IsFrightened() const { return m_IsFrightened; };
		void ClearFrightened() const { m_IsFrightened = false; m_PowerUpTimer = 0.f; };
		void SetGhostTexture() const;

		void TurnAround() const;
	protected:
		GhostAI(diji::GameObject* ownerPtr, diji::GameObject* player);

		std::unique_ptr<GhostState> m_CurrentStateUPtr = nullptr;
		glm::vec2 m_PersonnalSpawn = { 0, 0 };
		glm::vec2 m_ScatterTarget = { 0, 0 };
		std::string m_TexturePath = "";

	private:
		diji::Collider* m_PlayerColliderPtr;
		diji::Collider* m_ColliderCompPtr;
		diji::Transform* m_TransformCompPtr;
		diji::Texture* m_TextureCompPtr;
		std::unique_ptr<ChaseScatterAlgo> m_ChaseScatterAlgo = std::make_unique<ChaseScatterAlgo>();
		mutable float m_PowerUpTimer = 0.f;
		mutable bool m_IsFrightened = false;
	};

	class RedAI final : public GhostAI
	{
	public:
		RedAI(diji::GameObject* ownerPtr, diji::GameObject* player);
		~RedAI() noexcept = default;

		std::unique_ptr<GhostState> GetChaseState() const override;

		void Init() override;
	};
	
}

