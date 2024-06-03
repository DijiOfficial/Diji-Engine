#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "GameLoader.h"

using namespace diji;
void temp()
{
	//auto scene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::DEMO));

#pragma region Players
	//Player1
	//auto player = scene->CreateGameObject();
	//player->AddComponents<Texture>("pacmanSpriteSheet.png", 13, 13, 4);
	//player->AddComponents<Transform>(330, 250);
	//player->AddComponents<Render>();
	//player->AddComponents<pacman::HealthCounter>(3);
	//player->AddComponents<pacman::ScoreCounter>(0);

	////Player2
	//auto player2 = scene->CreateGameObject();
	//player2->AddComponents<Texture>("redGhost.png", 13, 13);
	//player2->AddComponents<Transform>(300, 250);
	//player2->AddComponents<Render>();
	//player2->AddComponents<pacman::HealthCounter>(3);
	//player2->AddComponents<pacman::ScoreCounter>(0);
#pragma endregion

#pragma region Controlls
	//auto& input = InputManager::GetInstance();
	//input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_W, player, Movement::Up);
	//input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_A, player, Movement::Left);
	//input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_S, player, Movement::Down);
	//input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_D, player, Movement::Right);
	//input.BindCommand<pacman::HitCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_C, player);
	//input.BindCommand<pacman::ScoreCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_Z, player, pacman::PointType::PickUp);
	//input.BindCommand<pacman::ScoreCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_X, player, pacman::PointType::Enemy);

	//input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadUp, player2, Movement::Up);
	//input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadLeft, player2, Movement::Left);
	//input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadDown, player2, Movement::Down);
	//input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadRight, player2, Movement::Right);
	//input.BindCommand<pacman::HitCommand>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::X, player2);
	//input.BindCommand<pacman::ScoreCommand>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::A, player2, pacman::PointType::PickUp);
	//input.BindCommand<pacman::ScoreCommand>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::B, player2, pacman::PointType::Enemy);
#pragma endregion

#pragma region HUD
	//auto smallFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	//auto HUD = scene->CreateGameObject();
	//HUD->AddComponents<Transform>(0, 70);

	//auto controllerInfo = scene->CreateGameObject();
	//controllerInfo->AddComponents<Transform>(0, 0);
	//controllerInfo->AddComponents<Text>("Use the D-Pad to move Pacman, X to inflict damage, A and B to pick up pellets", smallFont);
	//controllerInfo->AddComponents<Render>();
	//controllerInfo->SetParent(HUD, false);

	//auto controlsInfo = scene->CreateGameObject();
	//controlsInfo->AddComponents<Transform>(0, 20);
	//controlsInfo->AddComponents<Text>("Use the WASD to move MS Pacman, C to inflict damage, Z and X to pick up pellets", smallFont);
	//controlsInfo->AddComponents<Render>();
	//controlsInfo->SetParent(HUD, false);

	//auto livesCounter = scene->CreateGameObject();
	//livesCounter->AddComponents<Transform>(0, 60);
	//livesCounter->AddComponents<pacman::HealthObserver>("# lives: 3", smallFont);
	//livesCounter->AddComponents<Render>();
	//livesCounter->SetParent(HUD, false);

	//auto scoreCounter = scene->CreateGameObject();
	//scoreCounter->AddComponents<Transform>(0, 80);
	//scoreCounter->AddComponents<pacman::ScoreObserver>("Score: 0", smallFont);
	//scoreCounter->AddComponents<Render>();
	//scoreCounter->SetParent(HUD, false);

	//auto livesCounter2 = scene->CreateGameObject();
	//livesCounter2->AddComponents<Transform>(0, 100);
	//livesCounter2->AddComponents<pacman::HealthObserver>("# lives: 3", smallFont);
	//livesCounter2->AddComponents<Render>();
	//livesCounter2->SetParent(HUD, false);

	//auto scoreCounter2 = scene->CreateGameObject();
	//scoreCounter2->AddComponents<Transform>(0, 120);
	//scoreCounter2->AddComponents<pacman::ScoreObserver>("Score: 0", smallFont);
	//scoreCounter2->AddComponents<Render>();
	//scoreCounter2->SetParent(HUD, false);
#pragma endregion

#pragma region Observers
	//player->GetComponent<pacman::HealthCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::HEALTH_CHANGE), livesCounter->GetComponent<pacman::HealthObserver>());
	//player2->GetComponent<pacman::HealthCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::HEALTH_CHANGE), livesCounter2->GetComponent<pacman::HealthObserver>());
	//player->GetComponent<pacman::ScoreCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::SCORE_CHANGE), scoreCounter->GetComponent<pacman::ScoreObserver>());
	//player2->GetComponent<pacman::ScoreCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::SCORE_CHANGE), scoreCounter2->GetComponent<pacman::ScoreObserver>());
#pragma endregion
}

int main(int, char*[]) {
	diji::Minigin engine("../Data/");
	engine.Run(Loader::Load);

	return 0;
}