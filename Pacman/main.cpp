#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


#include "Minigin.h"
//#include "Scene.h"
#include "Components.h"
#include "PickUpManager.h"
//#include "GameObject.h"
#include "InputManager.h"
#include "Subject.h"
#include "ISoundSystem.h"
#include "Collision.h"

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

	auto scene = SceneManager::GetInstance().CreateScene("Demo");

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
	player->AddComponents<HealthCounter>(3);
	player->AddComponents<ScoreCounter>(0);

	//Player2
	auto player2 = scene->CreateGameObject();
	player2->AddComponents<Texture>("redGhost.png", 13, 13);
	player2->AddComponents<Transform>(300, 250);
	player2->AddComponents<Render>();
	player2->AddComponents<HealthCounter>(3);
	player2->AddComponents<ScoreCounter>(0);
#pragma endregion

#pragma region Controlls
	auto& input = InputManager::GetInstance();
	input.BindCommand<Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_W, player, Movement::Up);
	input.BindCommand<Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_A, player, Movement::Left);
	input.BindCommand<Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_S, player, Movement::Down);
	input.BindCommand<Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_D, player, Movement::Right);
	input.BindCommand<HitCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_C, player);
	input.BindCommand<ScoreCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_Z, player, PointType::PickUp);
	input.BindCommand<ScoreCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_X, player, PointType::Enemy);

	input.BindCommand<Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadUp, player2, Movement::Up);
	input.BindCommand<Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadLeft, player2, Movement::Left);
	input.BindCommand<Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadDown, player2, Movement::Down);
	input.BindCommand<Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadRight, player2, Movement::Right);
	input.BindCommand<HitCommand>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::X, player2);
	input.BindCommand<ScoreCommand>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::A, player2, PointType::PickUp);
	input.BindCommand<ScoreCommand>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::B, player2, PointType::Enemy);
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
	livesCounter->AddComponents<HealthObserver>("# lives: 3", smallFont);
	livesCounter->AddComponents<Render>();
	livesCounter->SetParent(HUD, false);

	auto scoreCounter = scene->CreateGameObject();
	scoreCounter->AddComponents<Transform>(0, 80);
	scoreCounter->AddComponents<ScoreObserver>("Score: 0", smallFont);
	scoreCounter->AddComponents<Render>();
	scoreCounter->SetParent(HUD, false);

	auto livesCounter2 = scene->CreateGameObject();
	livesCounter2->AddComponents<Transform>(0, 100);
	livesCounter2->AddComponents<HealthObserver>("# lives: 3", smallFont);
	livesCounter2->AddComponents<Render>();
	livesCounter2->SetParent(HUD, false);

	auto scoreCounter2 = scene->CreateGameObject();
	scoreCounter2->AddComponents<Transform>(0, 120);
	scoreCounter2->AddComponents<ScoreObserver>("Score: 0", smallFont);
	scoreCounter2->AddComponents<Render>();
	scoreCounter2->SetParent(HUD, false);
#pragma endregion

#pragma region Observers
	player->GetComponent<HealthCounter>()->AddObserver(MessageTypes::HEALTH_CHANGE, livesCounter->GetComponent<HealthObserver>());
	player2->GetComponent<HealthCounter>()->AddObserver(MessageTypes::HEALTH_CHANGE, livesCounter2->GetComponent<HealthObserver>());
	player->GetComponent<ScoreCounter>()->AddObserver(MessageTypes::SCORE_CHANGE, scoreCounter->GetComponent<ScoreObserver>());
	player2->GetComponent<ScoreCounter>()->AddObserver(MessageTypes::SCORE_CHANGE, scoreCounter2->GetComponent<ScoreObserver>());
#pragma endregion
}

void Pacman()
{
#ifndef NDEBUG
	ServiceLocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SDLISoundSystem>()));
#else
	ServiceLocator::RegisterSoundSystem(std::make_unique<SDLISoundSystem>());
#endif

	auto scene = SceneManager::GetInstance().CreateScene("Pacman");
	auto& input = InputManager::GetInstance();

	//Background
	auto background = scene->CreateGameObject();
	background->AddComponents<Texture>("BackgroundLevel.png");
	background->AddComponents<Transform>(0, 78);
	background->AddComponents<Render>(2);

	Collision::GetInstance().ParseLevelSVG("BackgroundLevelBlack.svg", 78);

	auto player = scene->CreateGameObject();
	player->AddComponents<Texture>("pacmanSpriteSheet5.png", 15, 15, 4);
	player->GetComponent<Texture>()->SetRotation(true);
	player->AddComponents<Transform>(214, 439);
	player->AddComponents<Render>(2);
	player->AddComponents<HealthCounter>(3);
	player->AddComponents<ScoreCounter>(0);
	player->AddComponents<Collider>(15, 15);
	player->AddComponents<AI>();

	//thought about making it just a temporary object, but what if I want to add more pickups (fruits, but they are recurring items so..)
	//or reset the level
	PickUpManager::GetInstance().Initialize(player);

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
	scoreCounter->AddComponents<ScoreObserver>("Score: 0", smallFont);
	scoreCounter->AddComponents<Render>();
	scoreCounter->SetParent(HUD, false);
#pragma endregion


#pragma region Commands
	input.BindCommand<Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_W, player, Movement::Up);
	input.BindCommand<Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_A, player, Movement::Left);
	input.BindCommand<Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_S, player, Movement::Down);
	input.BindCommand<Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_D, player, Movement::Right);
	input.BindCommand<HitCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_C, player);

	input.BindCommand<Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadUp, player, Movement::Up);
	input.BindCommand<Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadLeft, player, Movement::Left);
	input.BindCommand<Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadDown, player, Movement::Down);
	input.BindCommand<Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadRight, player, Movement::Right);
	input.BindCommand<HitCommand>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::X, player);
#pragma endregion

#pragma region Observers
	player->GetComponent<ScoreCounter>()->AddObserver(MessageTypes::SCORE_CHANGE, scoreCounter->GetComponent<ScoreObserver>());
#pragma endregion
}


int main(int, char*[]) {
	diji::Minigin engine("../Data/");
	//engine.Run(load);
	engine.Run(Pacman);
    return 0;
}