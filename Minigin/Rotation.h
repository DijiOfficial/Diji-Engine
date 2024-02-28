#pragma once
#include "GameObject.h"

namespace diji
{
    class GameObject;
    class Transform;

    class Rotation final : public Component
    {
    public:
        Rotation(GameObject* ownerPtr);
        Rotation(GameObject* ownerPtr, float rotationSpeed, float distanceFromCenter);

        Rotation(const Rotation& other) = delete;
        Rotation(Rotation&& other) = delete;
        Rotation& operator=(const Rotation& other) = delete;
        Rotation& operator=(Rotation&& other) = delete;

        void SetRotationAngle(float angle) { m_RotationAngle = angle; }
        float GetRotationAngle() const { return m_RotationAngle; }

        void SetRotationSpeed(float speed) { m_RotationSpeed = speed; }
        float GetRotationSpeed() const { return m_RotationSpeed; }

        void SetCenter(const glm::vec3& center) { m_Center = center; }
        const glm::vec3& GetCenter() const { return m_Center; }

        void SetDistanceFromCenter(float distance) { m_DistanceFromCenter = distance; }
        float GetDistanceFromCenter() const { return m_DistanceFromCenter; }

        void Update() override;

    private:
        float m_RotationAngle;
        float m_RotationSpeed;
        glm::vec3 m_Center;
        float m_DistanceFromCenter;
        Transform* m_TransformCompPtr{};
    };
}
