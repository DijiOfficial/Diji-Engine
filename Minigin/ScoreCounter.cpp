#include "ScoreCounter.h"
#include "Text.h"

#include <format>
#include <iostream>
diji::ScoreCounter::ScoreCounter(GameObject* ownerPtr, int score)
    : Component(ownerPtr)
    , m_Score{ score }
{

};


void diji::ScoreCounter::HitEnemy(PointType& pointType)
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

    auto& subject = ISubject::GetInstance();
    subject.Notify(GetOwner(), MessageTypes::SCORE_UPDATE);
}

void diji::ScoreCounter::Update()
{

}