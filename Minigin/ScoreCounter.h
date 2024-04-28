#pragma once
//#include "GameObject.h"
#include "Subject.h"

namespace diji
{
	enum class PointType
	{
		Enemy = 100,
		PickUp = 50,
		Pellet = 10
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
		void IncreaseScore(PointType& pointType);
		void IncreaseScore(const int score) { m_Score += score; };

		int GetScore() const { return m_Score; }

	private:
		int m_Score;
	};
}

