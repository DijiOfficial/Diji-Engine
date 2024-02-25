#pragma once
#include "Singleton.h"

namespace diji 
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
	};
}
