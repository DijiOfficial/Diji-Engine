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

	class Commands
	{

	public:
		virtual ~Commands() = default;

		Commands(const Commands& other) = delete;
		Commands(Commands&& other) = delete;
		Commands& operator=(const Commands& other) = delete;
		Commands& operator=(Commands&& other) = delete;

		virtual void Execute() = 0;

	protected:
		explicit Commands() = default;

	};
}
