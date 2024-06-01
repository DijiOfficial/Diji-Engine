#include "HealthCounter.h"
#include "ISoundSystem.h"
#include "Observers.h"
#include "SceneManager.h"
#include "GameState.h"

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
        diji::SceneManager::GetInstance().SetNextSceneToActivate(static_cast<int>(GameState::GAMEOVER));
        //diji::SceneManager::GetInstance().TranferScene(static_cast<int>(GameState::LEVEL), static_cast<int>(GameState::GAMEOVER), GetOwner());
        return;
    }

    Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::HEALTH_CHANGE));

}