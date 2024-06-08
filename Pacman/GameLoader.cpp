#include "GameLoader.h"

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
#include "EnterName.h"
#include "CustomTextRender.h"
#include "ScoreBoard.h"
#include "Fruit.h"

using namespace diji;

namespace loader
{
	constexpr glm::vec2 VIEWPORT{ 452, 608 };
}

void Loader::PacmanMenu()
{
	const auto& menuScene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::MENU));
	const auto& mediumFont = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 18);

	auto menuUI = menuScene->CreateGameObject("menuUI"); //will handle the drawings and AI part of the menu
	menuUI->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f, loader::VIEWPORT.y * 0.5f);
	menuUI->AddComponents<pacman::Menu>();
	const ShapeInfo shapeInfo = { Rectf{0, 0, 0, 0}, SDL_Color{235, 235, 235}, 1 };
	menuUI->AddComponents<Render>(shapeInfo);
	//menuUI->GetComponent<Render>()->SetIsShape();

	auto singlePlayer = menuScene->CreateGameObject("singlePLayerTexture");
	singlePlayer->AddComponents<Transform>(-120, 0);
	singlePlayer->AddComponents<Texture>("SinglePlayer.png", 15, 15);
	singlePlayer->AddComponents<Render>(2);
	singlePlayer->SetParent(menuUI, false);

	auto coop = menuScene->CreateGameObject("coopTexture");
	coop->AddComponents<Transform>(-45, 0);
	coop->AddComponents<Texture>("Coop.png", 30, 15);
	coop->AddComponents<Render>(2);
	coop->SetParent(menuUI, false);

	auto versus = menuScene->CreateGameObject("versusTexture");
	versus->AddComponents<Transform>(60, 0);
	versus->AddComponents<Texture>("Versus.png", 30, 15);
	versus->AddComponents<Render>(2);
	versus->SetParent(menuUI, false);

	auto pushStart = menuScene->CreateGameObject("startTextMenu");
	pushStart->AddComponents<Text>("PUSH ENTER BUTTON", mediumFont, SDL_Color{ 206, 176, 110, 255 }, true);
	pushStart->AddComponents<Transform>(0, 120);
	pushStart->AddComponents<Render>();
	pushStart->SetParent(menuUI, false);

	auto& input = InputManager::GetInstance();

	input.BindCommand<pacman::MenuSwitch>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::DPadLeft, menuUI, pacman::MenuSwitch::MenuButtons::Left);
	input.BindCommand<pacman::MenuSwitch>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::DPadRight, menuUI, pacman::MenuSwitch::MenuButtons::Right);
	input.BindCommand<pacman::MenuSwitch>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::Start, menuUI, pacman::MenuSwitch::MenuButtons::Enter);
	input.BindCommand<pacman::MenuSwitch>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::A, menuUI, pacman::MenuSwitch::MenuButtons::Enter);

}

void Loader::CommonGameAssets(Scene* &scene)
{
	Collision::GetInstance().Reset();

	auto background = scene->CreateGameObject("A_background");
	background->AddComponents<Texture>("BackgroundLevel.png");
	background->AddComponents<Transform>(0, 78);
	background->AddComponents<Render>(2);

	auto pelletCounter = scene->CreateGameObject("pelletCounter");
	pelletCounter->AddComponents<pacman::PelletObserver>();
	pelletCounter->AddComponents<pacman::PelletCounter>();

	auto player = scene->CreateGameObject("player");
	player->AddComponents<Texture>("pacmanSpriteSheet5.png", 15, 15, 4);
	player->GetComponent<Texture>()->SetRotation(true);
	player->AddComponents<Transform>(214, 439);
	player->AddComponents<Render>(2);
	player->AddComponents<pacman::HealthCounter>(4);
	player->AddComponents<pacman::ScoreCounter>(0);
	player->AddComponents<Collider>(15, 15);
	player->AddComponents<pacman::AI>();
	player->GetComponent<Render>()->DisableRender();
	player->GetComponent<Texture>()->PauseAnimation();
#pragma region LevelIntro
	const auto& font = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);
	auto levelIntro = scene->CreateGameObject("levelIntro");
	levelIntro->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f, 250.f);
	levelIntro->AddComponents<pacman::LevelIntro>();

	auto PlayerText = scene->CreateGameObject("playerTextIntro");
	PlayerText->AddComponents<Transform>(0, 0);
	PlayerText->AddComponents<Text>("PLAYER ONE", font, SDL_Color{ 6, 232, 244, 255 }, 1);
	PlayerText->AddComponents<pacman::IntroTextObserver>(pacman::MessageTypesDerived::LEVEL_BEGIN);
	PlayerText->AddComponents<Render>();
	PlayerText->SetParent(levelIntro, false);

	auto readyText = scene->CreateGameObject("readyTextIntro");
	readyText->AddComponents<Transform>(0, 96);
	readyText->AddComponents<Text>("READY!", font, SDL_Color{ 255, 255, 0, 255 }, 1);
	readyText->AddComponents<pacman::IntroTextObserver>(pacman::MessageTypesDerived::LEVEL_START);
	readyText->AddComponents<Render>();
	readyText->SetParent(levelIntro, false);
#pragma endregion
#pragma region Ghosts
	auto GhostTimers = scene->CreateGameObject("ghostTimers");
	GhostTimers->AddComponents<pacman::GhostsTimers>();

	auto Blinky = scene->CreateGameObject("z_Blinky");
	Blinky->AddComponents<Texture>("RedGhost.png", 15, 15, 2);
	Blinky->AddComponents<Transform>(212, 247);
	Blinky->AddComponents<Render>(2);
	Blinky->AddComponents<Collider>(15, 15);
	Blinky->AddComponents<pacman::RedAI>(player, pelletCounter, GhostTimers);
	Blinky->AddComponents<pacman::GhostCollision>(player);
	Blinky->GetComponent<Render>()->DisableRender();

	auto Pinky = scene->CreateGameObject("z_Pinky");
	Pinky->AddComponents<Texture>("Pinky.png", 15, 15, 2);
	Pinky->AddComponents<Transform>(212, 300);
	Pinky->AddComponents<Render>(2);
	Pinky->AddComponents<Collider>(15, 15);
	Pinky->AddComponents<pacman::Pinky>(player, pelletCounter, GhostTimers);
	Pinky->AddComponents<pacman::GhostCollision>(player);
	Pinky->GetComponent<Render>()->DisableRender();

	auto Inky = scene->CreateGameObject("z_Inky");
	Inky->AddComponents<Texture>("Inky.png", 15, 15, 2);
	Inky->AddComponents<Transform>(180, 300);
	Inky->AddComponents<Render>(2);
	Inky->AddComponents<Collider>(15, 15);
	Inky->AddComponents<pacman::Inky>(player, pelletCounter, GhostTimers, Blinky);
	Inky->AddComponents<pacman::GhostCollision>(player);
	Inky->GetComponent<Render>()->DisableRender();

	auto Clyde = scene->CreateGameObject("z_Clyde");
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
#pragma region HUD
	const auto& smallFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	const auto& textFont = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);

	auto HUD = scene->CreateGameObject("HUD");
	HUD->AddComponents<Transform>(10, 10);

	auto scoreCounter = scene->CreateGameObject("scoreCounterHUD");
	scoreCounter->AddComponents<Transform>(-5, 20);
	scoreCounter->AddComponents<pacman::ScoreObserver>("     00", textFont);
	scoreCounter->AddComponents<Render>();
	scoreCounter->SetParent(HUD, false);

	auto playerText = scene->CreateGameObject("playerTextHUD");
	playerText->AddComponents<Transform>(65, 0);
	playerText->AddComponents<Text>("1UP", textFont, SDL_Color{ 255, 255, 255, 255 }, true);
	playerText->AddComponents<Render>();
	playerText->SetParent(HUD, false);

	auto highScoreText = scene->CreateGameObject("highScoreTextHUD");
	highScoreText->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f - 10, 0.f);
	highScoreText->AddComponents<Text>("HIGH SCORE", textFont, SDL_Color{ 255, 255, 255, 255 }, true);
	highScoreText->AddComponents<Render>();
	highScoreText->SetParent(HUD, false);

	auto highScoreDisplay = scene->CreateGameObject("highScoreDisplayHUD");
	highScoreDisplay->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f - 75.f, 20.f);
	highScoreDisplay->AddComponents<pacman::HighScoreObserver>("  ", textFont, SDL_Color{ 255, 255, 255, 255 }, false);
	highScoreDisplay->AddComponents<Render>();
	highScoreDisplay->SetParent(HUD, false);

	auto livesCounter = scene->CreateGameObject("livesCounterHUD");
	livesCounter->AddComponents<Texture>("Lives.png", 80, 11, 1);
	livesCounter->GetComponent<Texture>()->PauseAnimation();
	livesCounter->AddComponents<Transform>(42, 581);
	livesCounter->AddComponents<pacman::PacmanHealthObserver>();
	livesCounter->AddComponents<Render>(2);

	auto levelCounterHUD = scene->CreateGameObject("levelCounterHUD");
	levelCounterHUD->AddComponents<Texture>("LevelDisplay.png", 16, 16, 1);
	levelCounterHUD->GetComponent<Texture>()->PauseAnimation();
	levelCounterHUD->GetComponent<Texture>()->SetStartingFrame(17);
	levelCounterHUD->AddComponents<Transform>(388, 573);
	levelCounterHUD->AddComponents<pacman::LevelObserver>();
	levelCounterHUD->AddComponents<pacman::LevelCounterRender>(2);
#pragma endregion

	const auto& pointFont = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 8);

	auto fruit = scene->CreateGameObject("w_fruit");
	fruit->AddComponents<Text>("100", pointFont, SDL_Color{ 255, 183, 255, 255 }, false);
	fruit->AddComponents<Texture>("LevelDisplay.png", 16, 16, 1);
	fruit->GetComponent<Texture>()->SetStartingFrame(17);
	fruit->GetComponent<Texture>()->PauseAnimation();
	fruit->AddComponents<Transform>(214, 343);
	fruit->AddComponents<pacman::FruitRender>(2);
	fruit->GetComponent<pacman::FruitRender>()->DisableRender();
	fruit->AddComponents<Collider>(16, 16);
	fruit->AddComponents<pacman::Fruit>(player, pelletCounter, levelCounterHUD);

#pragma region Commands
	auto& input = InputManager::GetInstance();

	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_W, player, Movement::Up);
	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_A, player, Movement::Left);
	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_S, player, Movement::Down);
	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_D, player, Movement::Right);
	//input.BindCommand<pacman::HitCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_C, player);

	//input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadUp, player, Movement::Up);
	//input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadLeft, player, Movement::Left);
	//input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadDown, player, Movement::Down);
	//input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadRight, player, Movement::Right);
	//input.BindCommand<pacman::HitCommand>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::X, player);

	input.BindCommand<pacman::test>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_F1, pelletCounter);
#pragma endregion
#pragma region Observers
	//todo: reset audio as well and add quick reset animation
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), player->GetComponent<pacman::AI>());
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), Blinky->GetComponent<pacman::RedAI>());
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), Pinky->GetComponent<pacman::Pinky>());
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), Inky->GetComponent<pacman::Inky>());
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), Clyde->GetComponent<pacman::Clyde>());
	//pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), player->GetComponent<pacman::AI>());//one for level intro?
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), levelCounterHUD->GetComponent<pacman::LevelObserver>());

	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), PlayerText->GetComponent<pacman::IntroTextObserver>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_START), readyText->GetComponent<pacman::IntroTextObserver>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_START), GhostTimers->GetComponent<pacman::GhostsTimers>());

	player->GetComponent<pacman::HealthCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::HEALTH_CHANGE), livesCounter->GetComponent<pacman::PacmanHealthObserver>());
	player->GetComponent<pacman::ScoreCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::SCORE_CHANGE), scoreCounter->GetComponent<pacman::ScoreObserver>());
	player->GetComponent<pacman::ScoreCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::SCORE_CHANGE), highScoreDisplay->GetComponent<pacman::HighScoreObserver>());
	fruit->GetComponent<pacman::Fruit>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::FRUIT_COLLISION), player->GetComponent<pacman::AI>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), player->GetComponent<pacman::AI>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_START), player->GetComponent<pacman::AI>());

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

	//optional
	auto fpsCounter = scene->CreateGameObject("fpsCounter");
	fpsCounter->AddComponents<Text>("0 FPS", smallFont);
	fpsCounter->AddComponents<FPSCounter>();
	fpsCounter->AddComponents<Transform>(0, static_cast<int>(loader::VIEWPORT.y - 20));
	fpsCounter->AddComponents<Render>();
}

void Loader::PacmanLevel()
{
	auto scene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::LEVEL));

	CommonGameAssets(scene);
	//input.Reset();
	//Collision::GetInstance().Reset();

	//todo: ReadyText should spawn when player respawns
	//todo: highscore should display other scoresa

	const std::vector<GameObject*> ghosts = { scene->GetGameObject("z_Blinky"), scene->GetGameObject("z_Pinky"), scene->GetGameObject("z_Inky"), scene->GetGameObject("z_Clyde") };
	pacman::PickUpLoader pickUpLoader{ scene->GetGameObject("player"), ghosts, scene->GetGameObject("pelletCounter"), scene };

	//todo: add a second player to fruit with  a function
}

void Loader::VersusLevel()
{
	auto scene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::VERSUS));

	CommonGameAssets(scene);

	//const auto& Blinky = scene->GetGameObject("z_Blinky");

	//auto Blinky = scene->CreateGameObject("z_Blinky");
	//Blinky->AddComponents<Texture>("RedGhost.png", 15, 15, 2);
	//Blinky->AddComponents<Transform>(212, 247);
	//Blinky->AddComponents<Render>(2);
	//Blinky->AddComponents<Collider>(15, 15);
	//Blinky->AddComponents<pacman::RedAI>(player, pelletCounter, GhostTimers);
	//Blinky->AddComponents<pacman::GhostCollision>(player);
	//Blinky->GetComponent<Render>()->DisableRender();

	const auto& player2 = scene->GetGameObject("z_Blinky");
	player2->GetComponent<pacman::RedAI>()->SetAsPlayer();

	const std::vector<GameObject*> ghosts = { scene->GetGameObject("z_Blinky"), scene->GetGameObject("z_Pinky"), scene->GetGameObject("z_Inky"), scene->GetGameObject("z_Clyde") };
	pacman::PickUpLoader pickUpLoader{ scene->GetGameObject("player"), ghosts, scene->GetGameObject("pelletCounter"), scene };

#pragma region Input
	auto& input = InputManager::GetInstance();

	input.BindCommand<pacman::GhostSwitchState>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadUp, player2, Movement::Up);
	input.BindCommand<pacman::GhostSwitchState>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadLeft, player2, Movement::Left);
	input.BindCommand<pacman::GhostSwitchState>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadDown, player2, Movement::Down);
	input.BindCommand<pacman::GhostSwitchState>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadRight, player2, Movement::Right);
#pragma endregion
}

void Loader::CoopLevel()
{
	auto scene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::COOP));

	CommonGameAssets(scene);

	auto player2 = scene->CreateGameObject("player2");
	player2->AddComponents<Texture>("pacmanSpriteSheet5.png", 15, 15, 4);
	player2->GetComponent<Texture>()->SetRotation(true);
	player2->AddComponents<Transform>(214, 439);
	player2->GetComponent<Transform>()->SetMovement(diji::Movement::Left);
	player2->AddComponents<Render>(2);
	player2->AddComponents<pacman::HealthCounter>(4);
	player2->AddComponents<pacman::ScoreCounter>(0);
	player2->GetComponent<pacman::ScoreCounter>()->SetAsPlayer2();
	player2->AddComponents<Collider>(15, 15);
	player2->AddComponents<pacman::AI>();
	player2->GetComponent<pacman::AI>()->SetInitialMovementLeft();
	player2->GetComponent<Render>()->DisableRender();
	player2->GetComponent<Texture>()->PauseAnimation();
	player2->GetComponent<Texture>()->SetRotationAngle(180.f);
	

	const auto& Blinky = scene->GetGameObject("z_Blinky");
	const auto& Pinky = scene->GetGameObject("z_Pinky");
	const auto& Inky = scene->GetGameObject("z_Inky");
	const auto& Clyde = scene->GetGameObject("z_Clyde");

	Blinky->GetComponent<pacman::GhostCollision>()->AddPLayer2Collider(player2->GetComponent<Collider>());
	Pinky->GetComponent<pacman::GhostCollision>()->AddPLayer2Collider(player2->GetComponent<Collider>());
	Inky->GetComponent<pacman::GhostCollision>()->AddPLayer2Collider(player2->GetComponent<Collider>());
	Clyde->GetComponent<pacman::GhostCollision>()->AddPLayer2Collider(player2->GetComponent<Collider>());

	const std::vector<GameObject*> ghosts = { Blinky, Pinky, Inky, Clyde };
	pacman::PickUpLoader pickUpLoader{ scene->GetGameObject("player"), ghosts, scene->GetGameObject("pelletCounter"), scene, player2 };

	const auto& pelletCounter = scene->GetGameObject("pelletCounter");
	//const auto& player = scene->GetGameObject("player");
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), player2->GetComponent<pacman::AI>());

	const auto& fruit = scene->GetGameObject("w_fruit");
	fruit->GetComponent<pacman::Fruit>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::FRUIT_COLLISION), player2->GetComponent<pacman::AI>());
	fruit->GetComponent<pacman::Fruit>()->AddPlayer2Collider(player2->GetComponent<Collider>());

	const auto& levelIntro = scene->GetGameObject("levelIntro");
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), player2->GetComponent<pacman::AI>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_START), player2->GetComponent<pacman::AI>());

	Blinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player2->GetComponent<pacman::AI>());
	Pinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player2->GetComponent<pacman::AI>());
	Inky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player2->GetComponent<pacman::AI>());
	Clyde->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player2->GetComponent<pacman::AI>());

#pragma region Input
	auto& input = InputManager::GetInstance();

	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadUp, player2, Movement::Up);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadLeft, player2, Movement::Left);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadDown, player2, Movement::Down);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadRight, player2, Movement::Right);
#pragma endregion
}

void Loader::HighScoreMenu(int score)
{
	InputManager::GetInstance().ResetCommands();
	const auto& scene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::GAMEOVER));
	const auto& mediumFont = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 18);

	auto gameOverText = scene->CreateGameObject("gameOverText");
	gameOverText->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f, 120.f);
	gameOverText->AddComponents<Text>("GAME OVER!", mediumFont, SDL_Color{ 255, 255, 255, 255 }, true);
	gameOverText->AddComponents<Render>();

	auto enterYourName = scene->CreateGameObject("enterYourNameText");
	enterYourName->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f, 200.f);
	enterYourName->AddComponents<Text>("ENTER YOUR NAME:", mediumFont, SDL_Color{ 255, 255, 255, 255 }, true);
	enterYourName->AddComponents<Render>();

	auto enterName = scene->CreateGameObject("name");
	enterName->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f, 260.f);
	enterName->AddComponents<Text>("AAA", mediumFont, SDL_Color{ 255, 255, 255, 255 }, true);
	enterName->AddComponents<pacman::EnterName>(score);
	enterName->AddComponents<pacman::CustomTextRender>();

	//input
	auto& input = InputManager::GetInstance();
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_W, enterName, Movement::Up);
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_A, enterName, Movement::Left);
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_S, enterName, Movement::Down);
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_D, enterName, Movement::Right);
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_UP, enterName, Movement::Up);
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_LEFT, enterName, Movement::Left);
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_DOWN, enterName, Movement::Down);
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_RIGHT, enterName, Movement::Right);

	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_RETURN, enterName, Movement::Idle);
}

void Loader::Load()
{
#ifndef NDEBUG
	ServiceLocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SDLISoundSystem>()));
#else
	ServiceLocator::RegisterSoundSystem(std::make_unique<SDLISoundSystem>());
#endif
	pacman::ScoreBoard::GetInstance().Init();

	PacmanMenu();
	//PacmanLevel();
	//CoopLevel();
	//VersusLevel();
	//HighScoreMenu();


	Collision::GetInstance().ParseLevelSVG("BackgroundLevelBlack.svg", 78);
	Collision::GetInstance().ParseIntersectionsSVG("Intersections.svg", 78);

	SceneManager::GetInstance().SetActiveScene(static_cast<int>(pacman::GameState::MENU));
}
