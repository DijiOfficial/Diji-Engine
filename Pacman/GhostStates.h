#pragma once
#include <memory>
#include <map>
#include <glm/vec2.hpp>
#include <array>

namespace diji
{
	enum class Movement;
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

		//todo:adjust speeds based on level and special speed for Blinky based on dots remaining
	protected:
		void SeekTarget(const GhostAI* ghost, const glm::vec2& target);
		void GoToTarget(const GhostAI* ghost, const glm::vec2& target);
		const glm::vec2 m_SpawnPoint = { 227, 262 };
		float m_Step = 1.875f;
		bool m_DisplayDirection = true;
	private:
		static constexpr std::array<glm::vec2, 4> m_BlockedIntersections = { {{202, 262}, {250, 262}, {202, 454}, {250, 454}} };
		bool m_TempLock = false;
		int m_LockedFrames = 0;

		void CalculateDirection(const GhostAI* ghost, const glm::vec2& target);
		glm::vec2 GetTargetTranslation(diji::Movement movement) const;

		diji::Movement ChooseRandomDirection(const std::map<diji::Movement, bool>& possibleDirections) const;
	};

	class Eaten final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Eaten() noexcept = default;

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

		void OnEnter(const GhostAI*) override {};
		void OnExit(const GhostAI* ghost) override;
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		const glm::vec2 m_OutsidePosition = { 212, 247 };
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
		void SwitchSpeed();
	private:
		bool m_IsUpdated = false;
		float m_FrightSpeed = 1.25f;
	};

	class Chase : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Chase() noexcept = default;

		virtual void OnEnter(const GhostAI*) override = 0;
		virtual void OnExit(const GhostAI*) override = 0;
		virtual std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override = 0;
	};

	class RedChase final : public Chase
	{
	public:
		using Chase::Chase;
		~RedChase() noexcept = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	};
}

