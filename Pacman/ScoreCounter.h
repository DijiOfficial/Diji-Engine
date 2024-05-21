#pragma once
#include "Subject.h"
#include "Component.h"

namespace pacman
{
	enum class PointType
	{
		Enemy = 100,
		PickUp = 50,
		Pellet = 10
	};

	class ScoreCounter final : public diji::Component, public diji::Subject
	{
	public:
		ScoreCounter(diji::GameObject* ownerPtr, int score);
		~ScoreCounter() override = default;

		ScoreCounter(const ScoreCounter& other) = delete;
		ScoreCounter(ScoreCounter&& other) = delete;
		ScoreCounter& operator=(const ScoreCounter& other) = delete;
		ScoreCounter& operator=(ScoreCounter&& other) = delete;

		void Update() override {};
		void FixedUpdate() override {};
		void IncreaseScore(PointType& pointType);
		void IncreaseScore(const int score);

		int GetScore() const { return m_Score; }

	private:
		int m_Score;
	};
}

