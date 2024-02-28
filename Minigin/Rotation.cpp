#include "Rotation.h"
#include "Time.h"
#include "Transform.h"

diji::Rotation::Rotation(GameObject* ownerPtr) 
    : Component(ownerPtr)
    , m_RotationAngle{ 0.0f }
    , m_RotationSpeed{ 1.0f }
    , m_Center{ 0.0f, 0.0f, 0.0f }
    , m_DistanceFromCenter{ 0.0f }
{
}

diji::Rotation::Rotation(GameObject* ownerPtr, float rotationSpeed, float distanceFromCenter)
    : Component(ownerPtr)
    , m_RotationAngle{ 0.0f }
    , m_RotationSpeed{ rotationSpeed }
    , m_Center{ 0.0f, 0.0f, 0.0f }
    , m_DistanceFromCenter{ distanceFromCenter }
{
}

void diji::Rotation::Update()
{
    if (m_Center == glm::vec3{ 0, 0, 0 })
        SetCenter(GetOwner()->GetLocalPosition());

    const auto parent{ GetOwner()->GetParent() };
    if (parent)
        SetCenter(parent->GetLocalPosition());

    m_RotationAngle += m_RotationSpeed * Time::GetInstance().GetDeltaTime();

    // Update position based on rotation around center
    auto transform = GetOwner()->GetComponent<Transform>();
    if (transform)
    {
        float radians = glm::radians(m_RotationAngle);
        float x = m_Center.x + m_DistanceFromCenter * cosf(radians);
        float y = m_Center.y + m_DistanceFromCenter * sinf(radians);
        transform->SetPosition(glm::vec3(x, y, 0));
    }
}