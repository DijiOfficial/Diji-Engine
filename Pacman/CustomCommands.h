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

	class MenuSwitch final : public diji::GameActorCommands
	{
	public:
		enum class MenuButtons
		{
			Left,
			Right,
			Enter
		};

		MenuSwitch(diji::GameObject* actor, MenuButtons value);
		~MenuSwitch() noexcept override = default;

		void Execute() override;
	private:
		const MenuButtons m_Button;
		pacman::Menu* m_MenuComponentPtr;	
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

	class test final : public diji::GameActorCommands
	{
	public:
		test(diji::GameObject* actor);
		~test() noexcept override = default;

		void Execute() override;
	private:
		PelletCounter* tester;
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

