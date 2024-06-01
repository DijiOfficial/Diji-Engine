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
#include "LevelIntro.h"
#include "PelletCounter.h"
#include "Menu.h"

using namespace diji;
void temp()
{
	auto scene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::DEMO));

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

void PacmanMenu()
{ 
	constexpr glm::vec2 viewport{ 452, 608 };

	const auto& menuScene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::MENU));
	const auto& mediumFont = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 18);
	
	auto menuUI = menuScene->CreateGameObject(); //will handle the drawings and AI part of the menu
	menuUI->AddComponents<Transform>(viewport.x * 0.5f, viewport.y * 0.5f);
	menuUI->AddComponents<pacman::Menu>();
	const ShapeInfo shapeInfo = { Rectf{0, 0, 0, 0}, SDL_Color{235, 235, 235}, 1 };
	menuUI->AddComponents<Render>(shapeInfo);
	//menuUI->GetComponent<Render>()->SetIsShape();

	auto singlePlayer = menuScene->CreateGameObject();
	singlePlayer->AddComponents<Transform>(-120, 0);
	singlePlayer->AddComponents<Texture>("SinglePlayer.png", 15 , 15);
	singlePlayer->AddComponents<Render>(2);
	singlePlayer->SetParent(menuUI, false);

	auto coop = menuScene->CreateGameObject();
	coop->AddComponents<Transform>(-45, 0);
	coop->AddComponents<Texture>("Coop.png", 30, 15);
	coop->AddComponents<Render>(2);
	coop->SetParent(menuUI, false);

	auto versus = menuScene->CreateGameObject();
	versus->AddComponents<Transform>(60, 0);
	versus->AddComponents<Texture>("Versus.png", 30, 15);
	versus->AddComponents<Render>(2);
	versus->SetParent(menuUI, false);

	auto pushStart = menuScene->CreateGameObject();
	pushStart->AddComponents<Text>("PUSH ENTER BUTTON", mediumFont, SDL_Color{ 206, 176, 110, 255 }, true);
	pushStart->AddComponents<Transform>(0, 120);
	pushStart->AddComponents<Render>();
	pushStart->SetParent(menuUI, false);

}

void Pacman()
{
	constexpr glm::vec2 viewport{ 452, 608 };

	auto scene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::LEVEL));
	auto& input = InputManager::GetInstance();
	//Background
	auto background = scene->CreateGameObject();
	background->AddComponents<Texture>("BackgroundLevel.png");
	background->AddComponents<Transform>(0, 78);
	background->AddComponents<Render>(2);

	Collision::GetInstance().ParseLevelSVG("BackgroundLevelBlack.svg", 78);
	Collision::GetInstance().ParseIntersectionsSVG("Intersections.svg", 78);
	//todo: ReadyText should spawn when player respawns
	//todo: Add Fruits
	//todo: add lives when 4 ghost eatne?
#pragma region LevelIntro
	const auto& font = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);
	auto levelIntro = scene->CreateGameObject();
	levelIntro->AddComponents<Transform>(viewport.x * 0.5f, 250.f);
	levelIntro->AddComponents<pacman::LevelIntro>();

	auto PlayerText = scene->CreateGameObject();
	PlayerText->AddComponents<Transform>(0, 0);
	PlayerText->AddComponents<Text>("PLAYER ONE", font, SDL_Color{ 6, 232, 244, 255 }, 1);
	PlayerText->AddComponents<pacman::IntroTextObserver>(pacman::MessageTypesDerived::LEVEL_BEGIN);
	PlayerText->AddComponents<Render>();
	PlayerText->SetParent(levelIntro, false);

	auto readyText = scene->CreateGameObject();
	readyText->AddComponents<Transform>(0, 96);
	readyText->AddComponents<Text>("READY!", font, SDL_Color{ 255, 255, 0, 255 }, 1);
	readyText->AddComponents<pacman::IntroTextObserver>(pacman::MessageTypesDerived::LEVEL_START);
	readyText->AddComponents<Render>();
	readyText->SetParent(levelIntro, false);
#pragma endregion

	auto pelletCounter = scene->CreateGameObject();
	pelletCounter->AddComponents<pacman::PelletObserver>();
	pelletCounter->AddComponents<pacman::PelletCounter>();

	auto player = scene->CreateGameObject();
	player->AddComponents<Texture>("pacmanSpriteSheet5.png", 15, 15, 4);
	player->GetComponent<Texture>()->SetRotation(true);
	player->AddComponents<Transform>(214, 439);
	player->AddComponents<Render>(2);
	player->AddComponents<pacman::HealthCounter>(5);
	player->AddComponents<pacman::ScoreCounter>(0);
	player->AddComponents<Collider>(15, 15);
	player->AddComponents<pacman::AI>();
	player->GetComponent<Render>()->DisableRender();
	player->GetComponent<Texture>()->PauseAnimation();

#pragma region Ghosts
	auto GhostTimers = scene->CreateGameObject();
	GhostTimers->AddComponents<pacman::GhostsTimers>();

	auto Blinky = scene->CreateGameObject();
	Blinky->AddComponents<Texture>("RedGhost.png", 15, 15, 2);
	Blinky->AddComponents<Transform>(212, 247);
	Blinky->AddComponents<Render>(2);
	Blinky->AddComponents<Collider>(15, 15);
	Blinky->AddComponents<pacman::RedAI>(player, pelletCounter, GhostTimers);
	Blinky->AddComponents<pacman::GhostCollision>(player);
	Blinky->GetComponent<Render>()->DisableRender();

	auto Pinky = scene->CreateGameObject();
	Pinky->AddComponents<Texture>("Pinky.png", 15, 15, 2);
	Pinky->AddComponents<Transform>(212, 300);
	Pinky->AddComponents<Render>(2);
	Pinky->AddComponents<Collider>(15, 15);
	Pinky->AddComponents<pacman::Pinky>(player, pelletCounter, GhostTimers);
	Pinky->AddComponents<pacman::GhostCollision>(player);
	Pinky->GetComponent<Render>()->DisableRender();

	auto Inky = scene->CreateGameObject();
	Inky->AddComponents<Texture>("Inky.png", 15, 15, 2);
	Inky->AddComponents<Transform>(180, 300);
	Inky->AddComponents<Render>(2);
	Inky->AddComponents<Collider>(15, 15);
	Inky->AddComponents<pacman::Inky>(player, pelletCounter, GhostTimers, Blinky);
	Inky->AddComponents<pacman::GhostCollision>(player);
	Inky->GetComponent<Render>()->DisableRender();

	auto Clyde = scene->CreateGameObject();
	Clyde->AddComponents<Texture>("Clyde.png", 15, 15, 2);
	Clyde->AddComponents<Transform>(244, 300);
	Clyde->AddComponents<Render>(2);
	Clyde->AddComponents<Collider>(15, 15);
	Clyde->AddComponents<pacman::Clyde>(player, pelletCounter, GhostTimers);
	Clyde->AddComponents<pacman::GhostCollision>(player);
	Clyde->GetComponent<Render>()->DisableRender();

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
	const auto& smallFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	const auto& textFont = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);

	auto HUD = scene->CreateGameObject();
	HUD->AddComponents<Transform>(10, 10);

	auto scoreCounter = scene->CreateGameObject();
	scoreCounter->AddComponents<Transform>(-5, 20);
	scoreCounter->AddComponents<pacman::ScoreObserver>("     00", textFont);
	scoreCounter->AddComponents<Render>();
	scoreCounter->SetParent(HUD, false);

	auto playerText = scene->CreateGameObject();
	playerText->AddComponents<Transform>(65, 0);
	playerText->AddComponents<Text>("1UP", textFont, SDL_Color{ 255, 255, 255, 255 }, true);
	playerText->AddComponents<Render>();
	playerText->SetParent(HUD, false);

	auto highScoreText = scene->CreateGameObject();
	highScoreText->AddComponents<Transform>(viewport.x * 0.5f - 10, 0.f);
	highScoreText->AddComponents<Text>("HIGH SCORE", textFont, SDL_Color{ 255, 255, 255, 255 }, true);
	highScoreText->AddComponents<Render>();
	highScoreText->SetParent(HUD, false);

	auto highScoreDisplay = scene->CreateGameObject();
	highScoreDisplay->AddComponents<Transform>(viewport.x * 0.5f - 75.f, 20.f);
	highScoreDisplay->AddComponents<pacman::HighScoreObserver>("  ", textFont, SDL_Color{ 255, 255, 255, 255 }, false);
	highScoreDisplay->AddComponents<Render>();
	highScoreDisplay->SetParent(HUD, false);	

	auto livesCounter = scene->CreateGameObject();
	livesCounter->AddComponents<Texture>("Lives.png", 80, 11, 1);
	livesCounter->GetComponent<Texture>()->PauseAnimation();
	livesCounter->AddComponents<Transform>(32, 581);
	livesCounter->AddComponents<pacman::PacmanHealthObserver>();
	livesCounter->AddComponents<Render>(2);
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
	//todo: reset audio as well and add quick reset animation
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), player->GetComponent<pacman::AI>());
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), Blinky->GetComponent<pacman::RedAI>());
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), Pinky->GetComponent<pacman::Pinky>());
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), Inky->GetComponent<pacman::Inky>());
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), Clyde->GetComponent<pacman::Clyde>());
	//pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), player->GetComponent<pacman::AI>());//one for level intro?

	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), PlayerText->GetComponent<pacman::IntroTextObserver>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_START), readyText->GetComponent<pacman::IntroTextObserver>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_START), GhostTimers->GetComponent<pacman::GhostsTimers>());

	player->GetComponent<pacman::HealthCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::HEALTH_CHANGE), livesCounter->GetComponent<pacman::PacmanHealthObserver>());
	player->GetComponent<pacman::ScoreCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::SCORE_CHANGE), scoreCounter->GetComponent<pacman::ScoreObserver>());
	player->GetComponent<pacman::ScoreCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::SCORE_CHANGE), highScoreDisplay->GetComponent<pacman::HighScoreObserver>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), player->GetComponent<pacman::AI>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_START), player->GetComponent<pacman::AI>());
	
	//todo: rename redAI and AI
	Blinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Blinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Blinky->GetComponent<pacman::RedAI>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), Blinky->GetComponent<pacman::RedAI>());
	
	Pinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Pinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Pinky->GetComponent<pacman::Pinky>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), Pinky->GetComponent<pacman::Pinky>());
	
	Inky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Inky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Inky->GetComponent<pacman::Inky>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), Inky->GetComponent<pacman::Inky>());
	
	Clyde->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Clyde->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Clyde->GetComponent<pacman::Clyde>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), Clyde->GetComponent<pacman::Clyde>());
#pragma endregion


	auto fpsCounter = scene->CreateGameObject();
	fpsCounter->AddComponents<Text>("0 FPS", smallFont);
	fpsCounter->AddComponents<FPSCounter>();
	fpsCounter->AddComponents<Transform>(0, static_cast<int>(viewport.y - 20));
	fpsCounter->AddComponents<Render>();
}

void HighScoreMenu() 
{

}

void load()
{
#ifndef NDEBUG
	ServiceLocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SDLISoundSystem>()));
#else
	ServiceLocator::RegisterSoundSystem(std::make_unique<SDLISoundSystem>());
#endif

	PacmanMenu();
	Pacman();
	HighScoreMenu();

	SceneManager::GetInstance().SetActiveScene(static_cast<int>(pacman::GameState::MENU));
}

int main(int, char*[]) {
	diji::Minigin engine("../Data/");
	engine.Run(load);
	//engine.Run(Pacman);
    return 0;
}