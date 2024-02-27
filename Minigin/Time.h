#pragma once
#include "Singleton.h"
#include <chrono>

namespace diji
{
    class Time : public Singleton<Time>
    {
    public:
        float GetDeltaTime() const { return m_DeltaTime; }
        void SetDeltaTime(float deltaTime) { m_DeltaTime = deltaTime; }

    private:
        float m_DeltaTime{ 0.0f };

        friend class Singleton<Time>;
        Time() = default;
    };
}