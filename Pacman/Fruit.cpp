#include "Fruit.h"

#include "Collider.h"
#include "GameObject.h"
#include "ISoundSystem.h"
#include "Observers.h"
#include "Render.h"
#include "TimeSingleton.h"
#include "Texture.h"
#include "CustomTextRender.h"

pacman::Fruit::Fruit(diji::GameObject* ownerPtr, const diji::GameObject* player, const diji::GameObject* pelletCounter, const diji::GameObject* levelObserver)
	: Component(ownerPtr)
{
	m_PelletCounter = pelletCounter ? pelletCounter->GetComponent<PelletObserver>() : nullptr;
	m_LevelObserver = levelObserver ? levelObserver->GetComponent<LevelObserver>() : nullptr;
	m_PlayerColliderPtr = player->GetComponent<diji::Collider>();
	m_OwnerColliderPtr = nullptr;
}

void pacman::Fruit::Init()
{
	const auto& ownerPtr = GetOwner();
	m_OwnerColliderPtr = ownerPtr->GetComponent<diji::Collider>();
}
//todo: remove scenamanager scene getter
//todo: player cannot swithc direction when outside of the map (or make map bigger)
void pacman::Fruit::Update()
{
	if (m_IsEaten)
	{
		m_Timer += diji::TimeSingleton::GetInstance().GetDeltaTime();
		if (m_Timer >= 2.f)
		{
			GetOwner()->GetComponent<FruitRender>()->DisableRender();
			GetOwner()->GetComponent<FruitRender>()->RenderText(false);
			m_IsDisabled = true;
			m_IsEaten = false;
			m_Timer = 0.f;
		}
		return;
	}

	if ((m_PelletCounter->GetPelletCount() - 70) % 100 == 0 and not m_ChangedFrame)
	{
		const int level = m_LevelObserver->GetLevel() < 13 ? m_LevelObserver->GetLevel() - 1 : 12;
		m_Value = static_cast<int>(m_FruitCycle[level]);

		const auto& texture = GetOwner()->GetComponent<diji::Texture>();
		texture->SetStartingFrame(m_FruitFrame.at(m_Value));
		GetOwner()->GetComponent<diji::Text>()->SetText(std::to_string(m_Value));
		GetOwner()->GetComponent<FruitRender>()->EnableRender();
		const auto& collider = GetOwner()->GetComponent<diji::Collider>();
		diji::Collision::GetInstance().AddCollider(collider, collider->GetCollisionBox());

		m_IsDisabled = false;
		m_ChangedFrame = true;
	}

	if (m_ChangedFrame and (m_PelletCounter->GetPelletCount() - 70) % 100 != 0)
		m_ChangedFrame = false;

	if (m_IsDisabled)
		return;

	m_Timer += diji::TimeSingleton::GetInstance().GetDeltaTime();
	if (m_Timer >= 10.f)
	{
		m_IsDisabled = true;
		m_Timer = 0.f;
		GetOwner()->GetComponent<FruitRender>()->DisableRender();
		return;
	}

	const auto& colliders = diji::Collision::GetInstance().IsColliding(m_OwnerColliderPtr);
	for (const auto& collider : colliders)
	{
		if (collider == m_PlayerColliderPtr or collider == m_Player2ColliderPtr)
		{
			const auto& playerBox = collider == m_PlayerColliderPtr ? m_PlayerColliderPtr->GetCollisionBox() : m_Player2ColliderPtr->GetCollisionBox();
			const glm::vec2 playerCenter = { playerBox.left + playerBox.width * 0.5f, playerBox.bottom + playerBox.height * 0.5f };

			const auto& pelletBox = m_OwnerColliderPtr->GetCollisionBox();
			const glm::vec2 pelletCenter = { pelletBox.left + pelletBox.width * 0.5f, pelletBox.bottom + pelletBox.height * 0.5f };

			const float deltaX = playerCenter.x - pelletCenter.x;
			const float deltaY = playerCenter.y - pelletCenter.y;
			const float distance = deltaX * deltaX + deltaY * deltaY;

			if (distance <= 64.f)
				HandleCollision();

			break;
		}
	}
}

void pacman::Fruit::OnNotify(diji::MessageTypes message, diji::Subject*)
{
	auto msg = static_cast<MessageTypesDerived>(message);
	if (msg == MessageTypesDerived::LEVEL_END)
	{
		const auto& collider = GetOwner()->GetComponent<diji::Collider>();
		diji::Collision::GetInstance().AddCollider(collider, collider->GetCollisionBox());
	}
}

void pacman::Fruit::HandleCollision()
{
	const auto& owner = GetOwner();
	//m_RenderCompPtr->DisableRender();
	GetOwner()->GetComponent<FruitRender>()->RenderText(true);

	//change texture
	m_IsEaten = true;
	m_Timer = 0.f;

	diji::Collision::GetInstance().RemoveCollider(owner->GetComponent<diji::Collider>());
	diji::ServiceLocator::GetSoundSystem().AddSoundRequest(diji::SoundId::PacmanEatFruit, -1);
	Notify(static_cast<diji::MessageTypes>(MessageTypesDerived::FRUIT_COLLISION));
	
}
