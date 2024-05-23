#pragma once
#include <string>
#include <functional>

namespace diji
{
	class Minigin final
	{
	public:
		explicit Minigin(const std::string& dataPath);
		~Minigin() noexcept;

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

		void Run(const std::function<void()>& load);

	private:
		static constexpr float FIXED_TIME_STEP{ 1 / 75.75f };
		//static constexpr float FIXED_TIME_STEP{ 1 / 151.5f };
	};
}
