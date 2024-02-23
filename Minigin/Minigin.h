#pragma once
#include <string>
#include <functional>

class Minigin
{
public:
	explicit Minigin(const std::string& dataPath);
	~Minigin();
	void Run(const std::function<void()>& load);

	Minigin(const Minigin& other) = delete;
	Minigin(Minigin&& other) = delete;
	Minigin& operator=(const Minigin& other) = delete;
	Minigin& operator=(Minigin&& other) = delete;

private:
	int GetMonitorRefreshRate();
	int m_RefreshRate{};
	long long m_TimePerFrame{};

	const float fixedTimeStep{ 20.f };
};
