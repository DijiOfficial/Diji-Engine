#pragma once
#include "GameObject.h"

namespace diji
{
	enum class Movement
	{
		Idle = 0,
		Up = 1,
		Down = 2,
		Left = 3,
		Right = 4,

		TotalMovements
	};

	class Command
	{
	public:
		Command() = default;
		virtual ~Command() = default;

		Command(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;

		virtual void Execute() = 0;
	
	private:
	};
}
