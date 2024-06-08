#include "HealthCounter.h"
#include "ISoundSystem.h"
#include "Observers.h"
#include "SceneManager.h"
#include "GameState.h"
#include "GameLoader.h"
#include "ScoreCounter.h"

pacman::HealthCounter::HealthCounter(diji::GameObject* ownerPtr, int health)
    : Component(ownerPtr)
    , m_Health{ health }
{
    
};


void pacman::HealthCounter::DecreaseHealth()
{
	--m_Health;

    if (m_Health < 0)
    {
        const auto& scoreCounter = GetOwner()->GetComponent<ScoreCounter>();
        if (scoreCounter->IsPlayer2())
            return;
        diji::SceneManager::GetInstance().SetNextSceneToActivate(static_cast<int>(GameState::GAMEOVER));
        Loader::HighScoreMenu(scoreCounter->GetScore());
        return;
    }

    Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::HEALTH_CHANGE));

}

void pacman::HealthCounter::AddLife()
{
    ++m_Health;
	Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::HEALTH_CHANGE));
}
