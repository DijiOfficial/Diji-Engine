#include "ScoreCounter.h"
#include "Observers.h"

diji::ScoreCounter::ScoreCounter(GameObject* ownerPtr, int score)
    : Component(ownerPtr)
    , m_Score{ score }
{

};

void diji::ScoreCounter::IncreaseScore(PointType& pointType)
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

    Notify(static_cast<MessageTypes>(MessageTypesDerived::SCORE_CHANGE));
}

void diji::ScoreCounter::IncreaseScore(const int score)
{
    m_Score += score;
	Notify(static_cast<MessageTypes>(MessageTypesDerived::SCORE_CHANGE));
}