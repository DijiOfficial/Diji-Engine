#pragma once
#include "Component.h"
#include "ChaseScatterAlgo.h"
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
	enum class Movement;
	class Texture;
	class AI;
	class GhostAI;
	class Chase;
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
		virtual std::unique_ptr<GhostState> Execute([[maybe_unused]] const GhostAI* ghost) = 0;

	protected:
		void SeekTarget(const GhostAI* ghost, const glm::vec2& target);
		void GoToTarget(const GhostAI* ghost, const glm::vec2& target);
		const glm::vec2 m_SpawnPoint{ 227, 262 };
		const int m_Step = 1;
		bool m_DisplayDirection = true;
	private:
		bool m_TempLock = false;
		int m_LockedFrames = 0;

		void CalculateDirection(const GhostAI* ghost, const glm::vec2& target);
		glm::vec2 GetTargetTranslation(Movement movement) const;

		Movement ChooseRandomDirection(const std::map<diji::Movement, bool>& possibleDirections) const;
	};
	class GhostAI : public Component
	{
	public:
		~GhostAI() = default;

		GhostAI(const GhostAI& other) = delete;
		GhostAI(GhostAI&& other) = delete;
		GhostAI& operator=(const GhostAI& other) = delete;
		GhostAI& operator=(GhostAI&& other) = delete;

		void Update() override { m_ChaseScatterAlgo->Update(); };
		void FixedUpdate() override;

		virtual std::unique_ptr<GhostState> GetChaseState() const = 0;

		Transform* GetTransform() const { return m_TransformCompPtr; };
		Collider* GetCollider() const { return m_ColliderCompPtr; };
		Collider* GetPlayerCollider() const { return m_PlayerColliderPtr; };
		Texture* GetTexture() const { return m_TextureCompPtr; };
		bool GetIsPoweredUp() const;
		bool GetIsInChaseState() const { return m_ChaseScatterAlgo->IsInChaseState(); };
		glm::vec2 GetSpawnPoint() const { return m_PersonnalSpawn; };
		glm::vec2 GetScatterTarget() const { return m_ScatterTarget; };

		void TurnAround() const;
	protected:
		GhostAI(GameObject* ownerPtr, GameObject* player);

		std::unique_ptr<GhostState> m_CurrentStateUPtr;
		glm::vec2 m_PersonnalSpawn{ 0, 0 };
		glm::vec2 m_ScatterTarget{ 0, 0 };

	private:
		Transform* m_TransformCompPtr;
		Collider* m_ColliderCompPtr;
		Collider* m_PlayerColliderPtr;
		Texture* m_TextureCompPtr;
		AI* m_PlayerAICompPtr;
		std::unique_ptr<ChaseScatterAlgo> m_ChaseScatterAlgo = std::make_unique<ChaseScatterAlgo>();
	};

	class Eaten final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Eaten() override = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		std::unique_ptr<GhostState> m_NextStateUPtr;
	};

	class Respawn final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Respawn() override = default;

		void OnEnter(const GhostAI* ghost) override { m_PersonnalSpawn = ghost->GetSpawnPoint(); };
		void OnExit(const GhostAI* ghost) override;
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		glm::vec2 m_PersonnalSpawn{ 0, 0 };
	};

	class ExitMaze final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~ExitMaze() override = default;

		void OnEnter(const GhostAI*) override {};
		void OnExit(const GhostAI* ghost) override;
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		const glm::vec2 m_OutsidePosition{ 212, 247 };
		std::unique_ptr<GhostState> m_NextStateUPtr;
	};

	class Scatter final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Scatter() override = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		glm::vec2 m_Target{ 0, 0 };
	};

	class Frightened final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Frightened() override = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI* ghost) override;
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	};

	class Chase : public GhostState
	{
	public:
		using GhostState::GhostState;
		virtual ~Chase() override = default;

		virtual void OnEnter(const GhostAI*) override = 0;
		virtual void OnExit(const GhostAI*) override = 0;
		virtual std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override = 0;
	};

	class RedChase final : public Chase
	{
	public:
		using Chase::Chase;
		~RedChase() override = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	};
}

