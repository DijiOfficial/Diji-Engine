#pragma once
//#include "GameObject.h"
#include "Subject.h"
namespace diji
{
	enum class PointType
	{
		Enemy = 100,
		PickUp = 50,
	};

	class ScoreCounter final : public Component, public Subject
	{
	public:
		ScoreCounter(GameObject* ownerPtr, int score);
		~ScoreCounter() override = default;

		ScoreCounter(const ScoreCounter& other) = delete;
		ScoreCounter(ScoreCounter&& other) = delete;
		ScoreCounter& operator=(const ScoreCounter& other) = delete;
		ScoreCounter& operator=(ScoreCounter&& other) = delete;

		void Update() override;
		void HitEnemy(PointType& pointType);

		int GetScore() const { return m_Score; }

	private:
		int m_Score;
	};
}

