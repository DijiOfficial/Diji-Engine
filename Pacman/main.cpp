#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Components.h"
#include "PickUpLoader.h"
#include "InputManager.h"
#include "ISoundSystem.h"
#include "Collision.h"
#include "CustomCommands.h"
#include "FPSCounter.h"
#include "ScoreCounter.h"
#include "HealthCounter.h"
#include "AI.h"
//#include "PinkAI.h"
#include "GhostAI.h"
#include "Observers.h"
#include "GhostCollision.h"
#include "GhostsAlgorithm.h"
#include "GameState.h"

using namespace diji;
void load()
{

#ifndef NDEBUG
	ServiceLocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SDLISoundSystem>()));
#else
	ServiceLocator::RegisterSoundSystem(std::make_unique<SDLISoundSystem>());
#endif

	//auto& ss = ServiceLocator::GetSoundSystem();
	//ss.PlaySound(SoundId::PacmanDie, -1);

	auto scene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::DEMO));

#pragma region Base
	//Background
	auto background = scene->CreateGameObject();
	background->AddComponents<Texture>("background.tga");
	background->AddComponents<Render>();

	//Logo
	auto logo = scene->CreateGameObject();
	logo->AddComponents<Texture>("logo.tga");
	logo->AddComponents<Transform>(300, 140);
	logo->AddComponents<Render>();

	//Title
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
	auto title = scene->CreateGameObject();
	title->AddComponents<Text>("Programming 4 Assignment", font);
	title->AddComponents<Transform>(220, 20);
	title->AddComponents<Render>();

	//FPS Counter
	auto fpsCounter = scene->CreateGameObject();
	fpsCounter->AddComponents<Text>("0 FPS", font);
	fpsCounter->AddComponents<FPSCounter>();
	fpsCounter->AddComponents<Transform>(0, 10);
	fpsCounter->AddComponents<Render>();
#pragma endregion

#pragma region Players
	//Player1
	auto player = scene->CreateGameObject();
	player->AddComponents<Texture>("pacmanSpriteSheet.png", 13, 13, 4);
	player->AddComponents<Transform>(330, 250);
	player->AddComponents<Render>();
	player->AddComponents<pacman::HealthCounter>(3);
	player->AddComponents<pacman::ScoreCounter>(0);

	//Player2
	auto player2 = scene->CreateGameObject();
	player2->AddComponents<Texture>("redGhost.png", 13, 13);
	player2->AddComponents<Transform>(300, 250);
	player2->AddComponents<Render>();
	player2->AddComponents<pacman::HealthCounter>(3);
	player2->AddComponents<pacman::ScoreCounter>(0);
#pragma endregion

#pragma region Controlls
	auto& input = InputManager::GetInstance();
	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_W, player, Movement::Up);
	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_A, player, Movement::Left);
	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_S, player, Movement::Down);
	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_D, player, Movement::Right);
	input.BindCommand<pacman::HitCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_C, player);
	input.BindCommand<pacman::ScoreCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_Z, player, pacman::PointType::PickUp);
	input.BindCommand<pacman::ScoreCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_X, player, pacman::PointType::Enemy);

	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadUp, player2, Movement::Up);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadLeft, player2, Movement::Left);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadDown, player2, Movement::Down);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadRight, player2, Movement::Right);
	input.BindCommand<pacman::HitCommand>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::X, player2);
	input.BindCommand<pacman::ScoreCommand>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::A, player2, pacman::PointType::PickUp);
	input.BindCommand<pacman::ScoreCommand>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::B, player2, pacman::PointType::Enemy);
#pragma endregion

#pragma region HUD
	auto smallFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	auto HUD = scene->CreateGameObject();
	HUD->AddComponents<Transform>(0, 70);

	auto controllerInfo = scene->CreateGameObject();
	controllerInfo->AddComponents<Transform>(0, 0);
	controllerInfo->AddComponents<Text>("Use the D-Pad to move Pacman, X to inflict damage, A and B to pick up pellets", smallFont);
	controllerInfo->AddComponents<Render>();
	controllerInfo->SetParent(HUD, false);

	auto controlsInfo = scene->CreateGameObject();
	controlsInfo->AddComponents<Transform>(0, 20);
	controlsInfo->AddComponents<Text>("Use the WASD to move MS Pacman, C to inflict damage, Z and X to pick up pellets", smallFont);
	controlsInfo->AddComponents<Render>();
	controlsInfo->SetParent(HUD, false);

	auto livesCounter = scene->CreateGameObject();
	livesCounter->AddComponents<Transform>(0, 60);
	livesCounter->AddComponents<pacman::HealthObserver>("# lives: 3", smallFont);
	livesCounter->AddComponents<Render>();
	livesCounter->SetParent(HUD, false);

	auto scoreCounter = scene->CreateGameObject();
	scoreCounter->AddComponents<Transform>(0, 80);
	scoreCounter->AddComponents<pacman::ScoreObserver>("Score: 0", smallFont);
	scoreCounter->AddComponents<Render>();
	scoreCounter->SetParent(HUD, false);

	auto livesCounter2 = scene->CreateGameObject();
	livesCounter2->AddComponents<Transform>(0, 100);
	livesCounter2->AddComponents<pacman::HealthObserver>("# lives: 3", smallFont);
	livesCounter2->AddComponents<Render>();
	livesCounter2->SetParent(HUD, false);

	auto scoreCounter2 = scene->CreateGameObject();
	scoreCounter2->AddComponents<Transform>(0, 120);
	scoreCounter2->AddComponents<pacman::ScoreObserver>("Score: 0", smallFont);
	scoreCounter2->AddComponents<Render>();
	scoreCounter2->SetParent(HUD, false);
#pragma endregion

#pragma region Observers
	player->GetComponent<pacman::HealthCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::HEALTH_CHANGE), livesCounter->GetComponent<pacman::HealthObserver>());
	player2->GetComponent<pacman::HealthCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::HEALTH_CHANGE), livesCounter2->GetComponent<pacman::HealthObserver>());
	player->GetComponent<pacman::ScoreCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::SCORE_CHANGE), scoreCounter->GetComponent<pacman::ScoreObserver>());
	player2->GetComponent<pacman::ScoreCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::SCORE_CHANGE), scoreCounter2->GetComponent<pacman::ScoreObserver>());
#pragma endregion
}

void Pacman()
{
#ifndef NDEBUG
	ServiceLocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SDLISoundSystem>()));
#else
	ServiceLocator::RegisterSoundSystem(std::make_unique<SDLISoundSystem>());
#endif
	const glm::vec2 viewport{ 452, 576 };

	//#pragma region Menu Scene
	//	const auto& menuScene = SceneManager::GetInstance().CreateScene("Menu");
	//	const auto& mediumFont = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 18);
	//
	//	auto pushStart = menuScene->CreateGameObject();
	//	pushStart->AddComponents<Text>("PUSH START BUTTON", mediumFont, SDL_Color{ 206, 176, 110, 255 }, true);
	//	pushStart->AddComponents<Transform>(viewport.x * 0.5f, viewport.y * 0.5f);
	//	pushStart->AddComponents<Render>();
	//
	//#pragma endregion

	SceneManager::GetInstance().SetActiveScene(static_cast<int>(pacman::GameState::LEVEL));
	auto scene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::LEVEL));
	auto& input = InputManager::GetInstance();
	//Background
	auto background = scene->CreateGameObject();
	background->AddComponents<Texture>("BackgroundLevel.png");
	background->AddComponents<Transform>(0, 78);
	background->AddComponents<Render>(2);

	Collision::GetInstance().ParseLevelSVG("BackgroundLevelBlack.svg", 78);
	Collision::GetInstance().ParseIntersectionsSVG("Intersections.svg", 78);

	auto pelletCounter = scene->CreateGameObject();
	pelletCounter->AddComponents<pacman::PelletObserver>();

	auto player = scene->CreateGameObject();
	player->AddComponents<Texture>("pacmanSpriteSheet5.png", 15, 15, 4);
	player->GetComponent<Texture>()->SetRotation(true);
	player->AddComponents<Transform>(214, 439);
	player->AddComponents<Render>(2);
	player->AddComponents<pacman::HealthCounter>(5);
	player->AddComponents<pacman::ScoreCounter>(0);
	player->AddComponents<Collider>(15, 15);
	player->AddComponents<pacman::AI>();
	//player->GetComponent<Render>()->EnableHitbox();

	auto livesCounter = scene->CreateGameObject();
	livesCounter->AddComponents<Texture>("Lives.png", 80, 11, 1);
	livesCounter->GetComponent<Texture>()->PauseAnimation();
	livesCounter->AddComponents<Transform>(32, 581);
	livesCounter->AddComponents<pacman::PacmanHealthObserver>();
	livesCounter->AddComponents<Render>(2);

	auto GhostTimers = scene->CreateGameObject();
	GhostTimers->AddComponents<pacman::GhostsTimers>();
#pragma region Ghosts
	auto Blinky = scene->CreateGameObject();
	Blinky->AddComponents<Texture>("RedGhost.png", 15, 15, 2);
	Blinky->AddComponents<Transform>(212, 247);
	Blinky->AddComponents<Render>(2);
	Blinky->AddComponents<Collider>(15, 15);
	Blinky->AddComponents<pacman::RedAI>(player, pelletCounter, GhostTimers);
	Blinky->AddComponents<pacman::GhostCollision>(player);

	auto Pinky = scene->CreateGameObject();
	Pinky->AddComponents<Texture>("Pinky.png", 15, 15, 2);
	Pinky->AddComponents<Transform>(212, 300);
	Pinky->AddComponents<Render>(2);
	Pinky->AddComponents<Collider>(15, 15);
	Pinky->AddComponents<pacman::Pinky>(player, pelletCounter, GhostTimers);
	Pinky->AddComponents<pacman::GhostCollision>(player);

	auto Inky = scene->CreateGameObject();
	Inky->AddComponents<Texture>("Inky.png", 15, 15, 2);
	Inky->AddComponents<Transform>(180, 300);
	Inky->AddComponents<Render>(2);
	Inky->AddComponents<Collider>(15, 15);
	Inky->AddComponents<pacman::Inky>(player, pelletCounter, GhostTimers, Blinky);
	Inky->AddComponents<pacman::GhostCollision>(player);

	auto Clyde = scene->CreateGameObject();
	Clyde->AddComponents<Texture>("Clyde.png", 15, 15, 2);
	Clyde->AddComponents<Transform>(244, 300);
	Clyde->AddComponents<Render>(2);
	Clyde->AddComponents<Collider>(15, 15);
	Clyde->AddComponents<pacman::Clyde>(player, pelletCounter, GhostTimers);
	Clyde->AddComponents<pacman::GhostCollision>(player);

	const std::vector<GameObject*> ghosts = { Blinky, Pinky, Inky, Clyde };

	//Set the ghosts aware of each other
	const auto& BlinkyAI = Blinky->GetComponent<pacman::RedAI>();
	const auto& PinkyAI = Pinky->GetComponent<pacman::Pinky>();
	const auto& InkyAI = Inky->GetComponent<pacman::Inky>();
	const auto& ClydeAI = Clyde->GetComponent<pacman::Clyde>();
	const std::vector<pacman::GhostAI*> ghostAIs = { BlinkyAI, PinkyAI, InkyAI, ClydeAI };
	BlinkyAI->SetGhostsVector(ghostAIs);
	PinkyAI->SetGhostsVector(ghostAIs);
	InkyAI->SetGhostsVector(ghostAIs);
	ClydeAI->SetGhostsVector(ghostAIs);
#pragma endregion
	pacman::PickUpLoader pickUpLoader{ player, ghosts, pelletCounter };
	//PickUpLoader::GetInstance().Initialize(player);

	//make the hud it's own scene?
#pragma region HUD
	auto smallFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	auto HUD = scene->CreateGameObject();
	HUD->AddComponents<Transform>(10, 10);

	auto controllerInfo = scene->CreateGameObject();
	controllerInfo->AddComponents<Transform>(0, 0);
	controllerInfo->AddComponents<Text>("Use the D-Pad to move Pacman, X to inflict damage", smallFont);
	controllerInfo->AddComponents<Render>();
	controllerInfo->SetParent(HUD, false);

	auto controlsInfo = scene->CreateGameObject();
	controlsInfo->AddComponents<Transform>(0, 20);
	controlsInfo->AddComponents<Text>("Use the WASD to move Pacman, C to inflict damage", smallFont);
	controlsInfo->AddComponents<Render>();
	controlsInfo->SetParent(HUD, false);

	auto scoreCounter = scene->CreateGameObject();
	scoreCounter->AddComponents<Transform>(0, 40);
	scoreCounter->AddComponents<pacman::ScoreObserver>("Score: 0", smallFont);
	scoreCounter->AddComponents<Render>();
	scoreCounter->SetParent(HUD, false);
#pragma endregion


#pragma region Commands
	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_W, player, Movement::Up);
	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_A, player, Movement::Left);
	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_S, player, Movement::Down);
	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_D, player, Movement::Right);
	input.BindCommand<pacman::HitCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_C, player);

	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadUp, player, Movement::Up);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadLeft, player, Movement::Left);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadDown, player, Movement::Down);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadRight, player, Movement::Right);
	input.BindCommand<pacman::HitCommand>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::X, player);


	input.BindCommand<pacman::HitCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_C, player);

#pragma endregion

#pragma region Observers
	player->GetComponent<pacman::HealthCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::HEALTH_CHANGE), livesCounter->GetComponent<pacman::PacmanHealthObserver>());
	player->GetComponent<pacman::ScoreCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::SCORE_CHANGE), scoreCounter->GetComponent<pacman::ScoreObserver>());
	Blinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Blinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Blinky->GetComponent<pacman::RedAI>());

	Pinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Pinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Pinky->GetComponent<pacman::Pinky>());

	Inky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Inky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Inky->GetComponent<pacman::Inky>());

	Clyde->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Clyde->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Clyde->GetComponent<pacman::Clyde>());
#pragma endregion


	auto fpsCounter = scene->CreateGameObject();
	fpsCounter->AddComponents<Text>("0 FPS", smallFont);
	fpsCounter->AddComponents<FPSCounter>();
	fpsCounter->AddComponents<Transform>(0, static_cast<int>(viewport.y - 20));
	fpsCounter->AddComponents<Render>();

	diji::ServiceLocator::GetSoundSystem().AddSoundRequest(diji::SoundId::Music, -1);
}


int main(int, char*[]) {
	diji::Minigin engine("../Data/");
	//engine.Run(load);
	engine.Run(Pacman);
    return 0;
}