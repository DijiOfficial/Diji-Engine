#include "Rotation.h"
#include "Time.h"
#include "Transform.h"

diji::Rotation::Rotation(GameObject* ownerPtr) 
    : Component(ownerPtr)
    , m_RotationAngle{ 0.0f }
    , m_RotationSpeed{ 1.0f }
    , m_Center{ 0.0f, 0.0f, 0.0f }
    , m_DistanceFromCenter{ 0.0f }
    , m_TransformCompPtr{ nullptr }
    , m_OwnerPtr{ ownerPtr }
{
}

diji::Rotation::Rotation(GameObject* ownerPtr, float rotationSpeed, float distanceFromCenter)
    : Component(ownerPtr)
    , m_RotationAngle{ 0.0f }
    , m_RotationSpeed{ rotationSpeed }
    , m_Center{ 0.0f, 0.0f, 0.0f }
    , m_DistanceFromCenter{ distanceFromCenter }
    , m_TransformCompPtr{ nullptr }
    , m_OwnerPtr{ ownerPtr }
{
}

void diji::Rotation::Update()
{
    Initialize();

    const auto parent{ m_OwnerPtr->GetParent() };
    if (parent)
        SetCenter(parent->GetWorldPosition());

    m_RotationAngle += m_RotationSpeed * Time::GetInstance().GetDeltaTime();

    if (m_TransformCompPtr)
    {
        float radians = glm::radians(m_RotationAngle);
        float x = m_Center.x + m_DistanceFromCenter * cosf(radians);
        float y = m_Center.y + m_DistanceFromCenter * sinf(radians);
        m_TransformCompPtr->SetPosition(glm::vec3(x, y, 0));
    }
}

void diji::Rotation::Initialize()
{
    if (not m_TransformCompPtr)
        m_TransformCompPtr = m_OwnerPtr->GetComponent<Transform>();

    if (m_Center == glm::vec3{ 0, 0, 0 })
        SetCenter(m_OwnerPtr->GetWorldPosition());
}
