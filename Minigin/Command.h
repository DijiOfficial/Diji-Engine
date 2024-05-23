#pragma once
namespace diji
{
	enum class Movement
	{
		Idle = -1,
		Right = 0,
		Down = 1,
		Left = 2,
		Up = 3,

		TotalMovements
	};

	class Command
	{
	public:
		Command() = default;
		virtual ~Command() noexcept = default;

		Command(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;

		virtual void Execute() = 0;
	};
}
