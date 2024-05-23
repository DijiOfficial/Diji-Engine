#pragma once
#include "Component.h"
#include "ChaseScatterAlgo.h"
#include "IObserver.h"

#include <glm/vec2.hpp>
#include <memory>

namespace diji
{
	class Transform;
	class Collider;
	enum class Movement;
	class Texture;
}

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
	
	class GhostState;

	class GhostAI : public diji::Component, public diji::IObserver
	{
	public:
		~GhostAI() = default;

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

		void TurnAround() const;
	protected:
		GhostAI(diji::GameObject* ownerPtr, diji::GameObject* player);

		std::unique_ptr<GhostState> m_CurrentStateUPtr;
		glm::vec2 m_PersonnalSpawn{ 0, 0 };
		glm::vec2 m_ScatterTarget{ 0, 0 };

	private:
		diji::Transform* m_TransformCompPtr;
		diji::Collider* m_ColliderCompPtr;
		diji::Collider* m_PlayerColliderPtr;
		diji::Texture* m_TextureCompPtr;
		std::unique_ptr<ChaseScatterAlgo> m_ChaseScatterAlgo = std::make_unique<ChaseScatterAlgo>();
		mutable bool m_IsFrightened = false;
		mutable float m_PowerUpTimer = 0.f;
	};

	class RedAI final : public GhostAI
	{
	public:
		RedAI(diji::GameObject* ownerPtr, diji::GameObject* player);
		~RedAI() = default;

		std::unique_ptr<GhostState> GetChaseState() const override;

		void Init() override;
	};
	
}

