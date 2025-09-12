#pragma once
#include <memory>
#include <map>
#include <glm/vec2.hpp>
#include <array>
#include "Command.h"

namespace diji
{
	enum class Movement;
	struct Rectf;
	class Collider;
	class Transform;
}

namespace pacman 
{
	class GhostAI;

	class GhostState
	{
	public:
		GhostState() = default;
		virtual ~GhostState() noexcept = default;

		GhostState(const GhostState& other) = delete;
		GhostState(GhostState&& other) = delete;
		GhostState& operator=(const GhostState& other) = delete;
		GhostState& operator=(GhostState&& other) = delete;

		virtual void OnEnter([[maybe_unused]] const GhostAI* ghost) = 0;
		virtual void OnExit([[maybe_unused]] const GhostAI* ghost) = 0;
		virtual std::unique_ptr<GhostState> Execute([[maybe_unused]] const GhostAI* ghost) = 0;

		void SetInTunnel(bool inTunnel) { m_IsInTunnel = inTunnel; };
		
	protected:
		void SeekTarget(const GhostAI* ghost, const glm::vec2& target);
		void GoToTarget(const GhostAI* ghost, const glm::vec2& target);
		const glm::vec2 m_SpawnPoint = { 227, 262 };
		float m_Step = 1.875f;
		bool m_DisplayDirection = true;
		
		void FreeModeMovement(const GhostAI* ghost);
		const diji::Rectf CalculateNewPosition(const diji::Movement& movement, diji::Collider* collider, diji::Transform* transform);
		diji::Movement m_PreviousMovement = diji::Movement::Right;

	private:
		static constexpr std::array<glm::vec2, 4> m_BlockedIntersections = { {{202, 262}, {250, 262}, {202, 454}, {250, 454}} };
		bool m_TempLock = false;
		bool m_IsInTunnel = false;
		int m_LockedFrames = 0;
		const float m_TunnelSpeed = 0.9375f;

		void CalculateDirection(const GhostAI* ghost, const glm::vec2& target);
		glm::vec2 GetTargetTranslation(diji::Movement movement) const;

		diji::Movement ChooseRandomDirection(const std::map<diji::Movement, bool>& possibleDirections) const;
	};

	class Waiting final : public GhostState
	{
	public:
		explicit Waiting(const int pellets);
		~Waiting() noexcept override = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI*) override;
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;

	private:
		const int m_PelletsNeeded;
		bool m_IsGoingUp = true;
		float m_WaitSpeed = 1.0f;
	};

	class Eaten final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Eaten() noexcept override = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI*) override;
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		float m_EatenSpeed = 3.25f;
	};

	class Respawn final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Respawn() noexcept = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI* ghost) override;
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		glm::vec2 m_PersonnalSpawn = { 0, 0 };
		float m_RespawnSpeed = 3.25f;
	};

	class ExitMaze final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~ExitMaze() noexcept = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI* ghost) override;
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		const glm::vec2 m_OutsidePosition = { 212.f, 247.f };
		float m_RespawnSpeed = 1.f;
	};

	class Scatter final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Scatter() noexcept = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		glm::vec2 m_Target = { 0, 0 };
	};

	class Frightened final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Frightened() noexcept = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI* ghost) override;
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;

		void ResetUpdate() { m_IsUpdated = false; }
	private:
		bool m_IsUpdated = false;
		float m_FrightSpeed = 1.25f;
	};

	class Chase : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Chase() noexcept = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(const GhostAI*) override = 0;
	};

	class RedChase final : public Chase
	{
	public:
		using Chase::Chase;
		~RedChase() noexcept = default;

		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	};

	class PinkyChase final : public Chase
	{
	public:
		using Chase::Chase;
		~PinkyChase() noexcept = default;

		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		const float m_TargetDistance = 64.f;
	};

	class InkyChase final : public Chase
	{
	public:
		using Chase::Chase;
		~InkyChase() noexcept = default;

		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		const float m_TargetDistance = 32.f;
	};

	class ClydeChase final : public Chase
	{
	public:
		using Chase::Chase;
		~ClydeChase() noexcept = default;

		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		const float m_TargetDistance = 128.f;
	};

	class Dying final : public GhostState
	{
	public:
		Dying(int points) : m_Points{ points } {};
		~Dying() noexcept = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI* ghost) override;
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	
	private:
		const int m_Points = 0;
	};
}

