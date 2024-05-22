#include "Rotation.h"

#include "TimeSingleton.h"
#include "Transform.h"
#include "GameObject.h"

diji::Rotation::Rotation(GameObject* ownerPtr) 
    : Component(ownerPtr)
    , m_OwnerPtr{ ownerPtr }
    , m_Center{ 0.0f, 0.0f, 0.0f }
    , m_RotationAngle{ 0.0f }
    , m_RotationSpeed{ 0.0f }
    , m_DistanceFromCenter{ 0.0f }
{
}

diji::Rotation::Rotation(GameObject* ownerPtr, float rotationSpeed, float distanceFromCenter)
    : Component(ownerPtr)
    , m_OwnerPtr{ ownerPtr }
    , m_Center{ 0.0f, 0.0f, 0.0f }
    , m_RotationAngle{ 0.0f }
    , m_RotationSpeed{ rotationSpeed }
    , m_DistanceFromCenter{ distanceFromCenter }
{ 
}

void diji::Rotation::Init()
{
    SetCenter(GetOwner()->GetComponent<Transform>()->GetPosition());
}

void diji::Rotation::Update()
{
    m_RotationAngle += m_RotationSpeed * TimeSingleton::GetInstance().GetDeltaTime();
    if (m_RotationAngle > TWO_PI_DEGRESS)
        m_RotationAngle -= TWO_PI_DEGRESS;

    const float radians = glm::radians(m_RotationAngle);
    const float x = m_DistanceFromCenter * cosf(radians);
    const float y = m_DistanceFromCenter * sinf(radians);

    if (m_OwnerPtr->GetParent())
        m_OwnerPtr->SetLocalPosition(glm::vec3(x, y, 0));
    else
        m_OwnerPtr->SetLocalPosition(m_Center + glm::vec3(x, y, 0));
}
