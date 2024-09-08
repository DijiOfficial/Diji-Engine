#pragma once
#include "GameActorCommands.h"
#include <glm/glm.hpp>
namespace diji
{
	class Transform;
	class GameObject;
}

namespace pacman
{
	class HealthCounter;
	class ScoreCounter;
	enum class PointType;
	class EnterName;
	class PelletCounter;
	class GhostAI;
	class Menu;

	class Move final : public diji::GameActorCommands
	{
	public:
		Move(diji::GameObject* actor, diji::Movement movement);
		~Move() noexcept override = default;

		void Execute() override;

	private:
		diji::Movement m_Movement;
		diji::Transform* m_TransformComponentPtr;
	};

	class MuteCommand final : public diji::GameActorCommands
	{
	public:
		MuteCommand(diji::GameObject* actor = nullptr);
		~MuteCommand() noexcept override = default;

		void Execute() override;

	private:
		bool m_IsMuted = false;
	};

	class MenuSwitch final : public diji::GameActorCommands
	{
	public:
		enum class MenuButtons
		{
			Left,
			Right,
			Up,
			Down,
			Enter
		};

		MenuSwitch(diji::GameObject* actor, MenuButtons value);
		~MenuSwitch() noexcept override = default;

		void Execute() override;
	private:
		const MenuButtons m_Button;
		pacman::Menu* m_MenuComponentPtr;	
	};

	class SingleCommands final : public diji::GameActorCommands
	{
	public:
		enum class SingleCommandOption
		{
			SKIP_INTRO,
		};

		SingleCommands(diji::GameObject* actor, SingleCommandOption option);
		~SingleCommands() noexcept override = default;

		void Execute() override;
	private:
		const SingleCommandOption m_Option;
	};

	class GhostSwitchState final : public diji::GameActorCommands
	{
	public:
		GhostSwitchState(diji::GameObject* actor, diji::Movement movement);
		~GhostSwitchState() noexcept override = default;

		void Execute() override;

	private:
		diji::Movement m_Movement;
		GhostAI* m_GhostAIComp;
	};

	class HitCommand final : public diji::GameActorCommands
	{

	public:
		HitCommand(diji::GameObject* actor);
		~HitCommand() noexcept override = default;

		void Execute() override;

	private:
		HealthCounter* m_HealthComponentPtr;
	};

	class NameChangeCommand final : public diji::GameActorCommands
	{ 
	public:
		NameChangeCommand(diji::GameObject* actor, diji::Movement movement);
		~NameChangeCommand() noexcept override = default;

		void Execute() override;

	private:
		EnterName* m_NameComp;
		diji::Movement m_Movement;

	};

	class SkipLevel final : public diji::GameActorCommands
	{
	public:
		SkipLevel(diji::GameObject* actor);
		~SkipLevel() noexcept override = default;

		void Execute() override;
	private:
		PelletCounter* m_PelletCounterPtr;
	};

	class ScoreCommand final : public diji::GameActorCommands
	{

	public:
		ScoreCommand(diji::GameObject* actorPtr, PointType point);
		~ScoreCommand() noexcept override = default;

		void Execute() override;

	private:
		ScoreCounter* m_ScoreComponentPtr;
		PointType m_PointType;
	};
}

