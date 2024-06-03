#include "ScoreCounter.h"
#include "Observers.h"
#include "GameObject.h"
#include "HealthCounter.h"
#include "ISoundSystem.h"

pacman::ScoreCounter::ScoreCounter(diji::GameObject* ownerPtr, int score)
    : Component(ownerPtr)
    , m_Score{ score }
{

};

void pacman::ScoreCounter::IncreaseScore(PointType& pointType)
{
    switch (pointType)
    {
        case PointType::Enemy:
            m_Score += static_cast<int>(PointType::Enemy);
            break;
        case PointType::PickUp:
            m_Score += static_cast<int>(PointType::PickUp);
            break;
        default:
            break;
    }

    Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::SCORE_CHANGE));

    CheckForExtraLife();

}

void pacman::ScoreCounter::IncreaseScore(const int score)
{
    m_Score += score;
	Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::SCORE_CHANGE));

    CheckForExtraLife();
}

void pacman::ScoreCounter::CheckForExtraLife()
{
    if (m_Extralife && m_Score >= 10000)
    {
        m_Extralife = false;
        GetOwner()->GetComponent<HealthCounter>()->AddLife();
        diji::ServiceLocator::GetSoundSystem().AddSoundRequest(diji::SoundId::ExtraLife, -1);
    }
}
