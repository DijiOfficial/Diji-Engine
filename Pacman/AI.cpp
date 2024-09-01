#include "AI.h"
#include "Texture.h"
#include "Transform.h"
#include "Collider.h"
#include "PickUp.h"
#include "ScoreCounter.h"
#include "GameObject.h"
#include "Observers.h"
#include "TimeSingleton.h"
#include "Render.h"
#include "ISoundSystem.h"
#include "GhostAI.h"
#include "HealthCounter.h"
#include "Fruit.h"
#include "GhostCollision.h"

namespace pacman {
    constexpr float TOTAL_WAIT_TIME = 2.54f;
    constexpr float PAUSE_DURATION = 1.0f;
    constexpr float DEATH_ANIMATION_TIME = 1.54f;
    constexpr int DEATH_ANIMATION_FRAMES = 13;
    constexpr float PAUSE_AI_TIME = 2.0f;
    constexpr int GHOST_EATEN_BASE_POINTS = 200;
    constexpr float PLAYER_MOVE_DISTANCE = 2.0f;
    constexpr glm::ivec2 ORIGINAL_POSITION{ 214, 439 };
    constexpr float TILE_SIZE = 20.0f;
    constexpr float TOTAL_WIDTH = 452.0f;

    AI::AI(diji::GameObject* ownerPtr)
        : Component(ownerPtr)
    {
        m_TextureCompPtr = nullptr;
        m_TransformCompPtr = nullptr;
        m_ColliderCompPtr = nullptr;
    }

    void AI::Init()
    {
        const auto& ownerPtr = GetOwner();
        m_TextureCompPtr = ownerPtr->GetComponent<diji::Texture>();
        m_TransformCompPtr = ownerPtr->GetComponent<diji::Transform>();
        m_ColliderCompPtr = ownerPtr->GetComponent<diji::Collider>();

        m_TransformCompPtr->SetMovement(m_PreviousMovement);
    }

    void AI::Update()
    {
        if (m_IsDying)
        {
            m_PauseTime += diji::TimeSingleton::GetInstance().GetDeltaTime();
            if (m_PauseTime >= PAUSE_DURATION && m_playeDeath)
            {
                m_TextureCompPtr->SetTexture("PacmanDeathSprite.png");
                m_TextureCompPtr->SetNrOfFrames(DEATH_ANIMATION_FRAMES);
                m_TextureCompPtr->SetCurrentFrame(0);
                m_TextureCompPtr->ResetFrameTime();
                m_TextureCompPtr->SetAnimationTime(DEATH_ANIMATION_TIME);
                m_TextureCompPtr->SetRotationAngle(0);
                m_TextureCompPtr->ResumeAnimation();

                diji::ServiceLocator::GetSoundSystem().AddSoundRequest("pacman_death.wav", false, -1);
                m_playeDeath = false;
            }
            if (!m_playeDeath && m_PauseTime >= TOTAL_WAIT_TIME)
            {
                Reset();
                GetOwner()->GetComponent<HealthCounter>()->DecreaseHealth();
            }
            return;
        }

        //clean up if statemetns
        if (m_PauseAI)
        {
            m_PauseTime += diji::TimeSingleton::GetInstance().GetDeltaTime();
            if (m_PauseTime >= PAUSE_AI_TIME)
            {
                m_PauseAI = false;
                m_PauseTime = 0.0f;
                GetOwner()->GetComponent<diji::Render>()->EnableRender();
                m_TextureCompPtr->ResumeAnimation();
            }
            else //huh?
            {
                return;
            }
        }
    }

    void AI::FixedUpdate()
    {
        if (m_PauseAI)
            return;

        const auto& currentMovement = m_TransformCompPtr->GetMovement();
        const auto& shape = CalculateNewPosition(currentMovement);

        if (m_IsInMenu or (CheckIfDirectionIsValid(currentMovement) and !diji::Collision::GetInstance().IsCollidingWithWorld(shape)))
        {
            m_TransformCompPtr->SetPosition(shape.left, shape.bottom);
            if (m_PreviousMovement != currentMovement)
            {
                if (m_PreviousMovement == diji::Movement::Idle)
                    m_TextureCompPtr->ResumeAnimation();
                if (currentMovement == diji::Movement::Idle)
                    m_TextureCompPtr->PauseAnimation();
                else
                    m_TextureCompPtr->SetRotationAngle(static_cast<int>(currentMovement) * 90.0f);

                m_PreviousMovement = currentMovement;
            }

            if (currentMovement != diji::Movement::Idle)
                m_TransformCompPtr->SetLookingDirection(currentMovement);
        }
        else
        {
            const auto& oldShape = CalculateNewPosition(m_PreviousMovement);

            if (!diji::Collision::GetInstance().IsCollidingWithWorld(oldShape))
                m_TransformCompPtr->SetPosition(oldShape.left, oldShape.bottom);
            else
                m_TransformCompPtr->SetMovement(diji::Movement::Idle);

            if (m_PreviousMovement != diji::Movement::Idle)
                m_TransformCompPtr->SetLookingDirection(m_PreviousMovement);
        }      
    }

    void AI::LateUpdate()
    {
        if (m_PauseAI)
            return;

        const auto& shape = m_ColliderCompPtr->GetCollisionBox();

        if (!m_TeleportedThisFrame)
        {
            if (shape.left < 0 - shape.width)
            {
                m_TransformCompPtr->SetPosition(TOTAL_WIDTH, shape.bottom);
                m_TeleportedThisFrame = true;
            }
            else if (shape.left > TOTAL_WIDTH)
            {
                m_TransformCompPtr->SetPosition(0 - shape.width, shape.bottom);
                m_TeleportedThisFrame = true;
            }
        }
        else
        {
            m_TeleportedThisFrame = false;
        }
    }

    void AI::OnNotify(diji::MessageTypes message, [[maybe_unused]] diji::Subject* subject)
    {
        if (m_IsDying)
            return;

        auto msg = static_cast<MessageTypesDerived>(message);
        switch (msg)
        {
        case MessageTypesDerived::POWERUP_COLLISION:
            if (m_IsInMenu)
            {
                m_TransformCompPtr->SetMovement(diji::Movement::Right);
                m_TextureCompPtr->SetRotationAngle(static_cast<int>(diji::Movement::Right) * 90.0f);
            }
            m_GhostsEaten = 0;
            [[fallthrough]];
        case MessageTypesDerived::PICKUP_COLLISION:
        {
            PickUp* pickUp = dynamic_cast<PickUp*>(subject);
            const int value = pickUp->GetValue();
            GetOwner()->GetComponent<ScoreCounter>()->IncreaseScore(value);
            break;
        }
        case MessageTypesDerived::FRUIT_COLLISION:
        {
            Fruit* fruit = dynamic_cast<Fruit*>(subject);
            const int value = fruit->GetValue();
            GetOwner()->GetComponent<ScoreCounter>()->IncreaseScore(value);
            break;
        }
        case MessageTypesDerived::ENEMY_COLLISION:
        {
            if (IsGhostFrightened(subject))
            {
                m_PauseAI = true;
                GetOwner()->GetComponent<diji::Render>()->DisableRender();
                const int value = GHOST_EATEN_BASE_POINTS * static_cast<int>(std::pow(2, m_GhostsEaten));
                GetOwner()->GetComponent<ScoreCounter>()->IncreaseScore(value);
                ++m_GhostsEaten;
            }
            else
            {
                m_IsDying = true;
                m_PauseAI = true;
                m_PauseTime = 0.0f;
            }
            break;
        }
        case MessageTypesDerived::LEVEL_BEGIN:
        {
            GetOwner()->GetComponent<diji::Render>()->EnableRender();
            GetOwner()->GetComponent<HealthCounter>()->DecreaseHealth();
            break;
        }
        case MessageTypesDerived::LEVEL_START:
        {
            m_PauseAI = false;
            m_PauseTime = 0.0f;
            m_TextureCompPtr->ResumeAnimation();
            break;
        }
        case MessageTypesDerived::LEVEL_END:
        {
            Reset();
            break;
        }
        case MessageTypesDerived::MENU_ANIMATION_BEGIN:
        {
            SetActive();
            m_TransformCompPtr->SetMovement(diji::Movement::Left);
            break;
        }
        default:
            break;
        }
    }

    void AI::SetActive()
    {
        m_PauseAI = false; 
        m_IsInMenu = true;
        m_PreviousMovement = diji::Movement::Left;
        m_SavedMovement = diji::Movement::Left;
        m_PauseTime = 0.0f;

    }
    const diji::Rectf AI::CalculateNewPosition(diji::Movement movement)
    {
        const auto position = m_TransformCompPtr->GetPosition() + glm::vec3{ m_TransformCompPtr->Get2DMovementVector(movement, PLAYER_MOVE_DISTANCE), 0.0f };
        auto shape = m_ColliderCompPtr->GetCollisionBox();
        shape.left = position.x;
        shape.bottom = position.y;

        if (not m_IsInMenu and diji::Collision::GetInstance().IsCollidingWithWorld(shape))
        {
            shape.left = std::round(shape.left);
            shape.bottom = std::round(shape.bottom);
            switch (movement)
            {
            case diji::Movement::Up:
                ++shape.bottom;
                break;
            case diji::Movement::Down:
                --shape.bottom;
                break;
            case diji::Movement::Left:
                ++shape.left;
                break;
            case diji::Movement::Right:
                --shape.left;
                break;
            }
        }

        return shape;
    }

    bool AI::IsGhostFrightened(diji::Subject* subject) const
    {
        const auto ghost = dynamic_cast<GhostCollision*>(subject);
        auto ghostAI = ghost->GetParent()->GetComponent<GhostAI>();
        return ghostAI->IsFrightened();
    }

    void AI::Reset()
    {
        m_TransformCompPtr->SetPosition(ORIGINAL_POSITION);
        m_TransformCompPtr->SetMovement(diji::Movement::Right);

        m_PreviousMovement = diji::Movement::Right;
        m_SavedMovement = diji::Movement::Right;

        m_TextureCompPtr->SetRotationAngle(static_cast<int>(diji::Movement::Right) * 90.0f);

        m_playeDeath = true;
        m_IsDying = false;
        m_PauseTime = 0.0f;
        m_PauseAI = true;
        m_GhostsEaten = 0;

        m_TextureCompPtr->PauseAnimation();
        m_TextureCompPtr->SetTexture("pacmanSpriteSheet5.png");
        m_TextureCompPtr->SetNrOfFrames(4);
        m_TextureCompPtr->SetCurrentFrame(3);
        m_TextureCompPtr->ResetFrameTime();
        m_TextureCompPtr->SetAnimationTime(0.3f);

        m_ColliderCompPtr->Update();

        diji::ServiceLocator::GetSoundSystem().AddSoundRequest("siren_1.wav", true, -1);
    }

    bool AI::CheckIfDirectionIsValid(const diji::Movement& movement)
    {
        const auto& playerShape = m_ColliderCompPtr->GetCollisionBox();
        const glm::vec2 playerCenter(playerShape.left + playerShape.width * 0.5f, playerShape.bottom + playerShape.height * 0.5f);
        const glm::vec2 movementVector = m_TransformCompPtr->Get2DMovementVector(movement, TILE_SIZE);

        glm::vec2 rayStart1, rayStart2, rayStart3;
        if (movement == diji::Movement::Up || movement == diji::Movement::Down)
        {
            rayStart1 = glm::vec2(playerShape.left, playerCenter.y);
            rayStart2 = glm::vec2(playerShape.left + playerShape.width, playerCenter.y);
            rayStart3 = playerCenter;
        }
        else
        {
            rayStart1 = glm::vec2(playerCenter.x, playerShape.bottom);
            rayStart2 = glm::vec2(playerCenter.x, playerShape.bottom + playerShape.height);
            rayStart3 = playerCenter;
        }

        const glm::vec2 rayEnd1 = rayStart1 + movementVector;
        const glm::vec2 rayEnd2 = rayStart2 + movementVector;
        const glm::vec2 rayEnd3 = rayStart3 + movementVector;

        if (diji::Collision::GetInstance().IsCollidingWithWorld(rayStart1, rayEnd1) ||
            diji::Collision::GetInstance().IsCollidingWithWorld(rayStart2, rayEnd2) ||
            diji::Collision::GetInstance().IsCollidingWithWorld(rayStart3, rayEnd3))
        {
            return false;
        }

        return true;
    }
}
