#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <chrono>
#include <thread>
#include <windows.h>

using namespace std;
SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	m_RefreshRate = GetMonitorRefreshRate();
	m_TimePerFrame = 1000 / m_RefreshRate;

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

int Minigin::GetMonitorRefreshRate()
{
	DEVMODE dm;
	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(dm);

	if (EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm) != 0)
	{
		return dm.dmDisplayFrequency;
	}

	return 0;
}

void Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();

	using clock = std::chrono::high_resolution_clock;
	bool doContinue = true;
	auto lastTime = clock::now();
	float lag = 0.0f;

	while (doContinue)
	{
		//account for lag
		const auto currentTime = clock::now();
		const float dt = chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;
		lag += dt;

		doContinue = input.ProcessInput();
		//fixed update are not needed yet
		//while (lag >= fixedTimeStep)
		//{
		//	fixed_update(fixedTimeStep);
		//	lag -= fixedTimeStep;
		//}

		sceneManager.Update();
		renderer.Render();
		//renderer.Render(lag/fixedTimeStep); //if wee need to render based on delta time, lag/fixedTimeStep tells us how far we are into the next frame (only matters if we skip over more than 1 frame I guess)
		//The renderer knows each game object and its current velocity. Say that bullet is 20 pixels from the left side of the screen and is moving right 400 pixels per frame. If we are halfway between frames, then we’ll end up passing 0.5 to render(). So it draws the bullet half a frame ahead, at 220 pixels. Ta-da, smooth motion.

		const auto sleepTime = currentTime + chrono::milliseconds(m_TimePerFrame) - clock::now();
		this_thread::sleep_for(sleepTime);
	}
}
