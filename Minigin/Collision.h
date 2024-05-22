#pragma once
#include "Singleton.h"
#include "SVGParser.h"
#include <map>

namespace diji
{
	class Collider;

	struct Rectf
	{
		float left, bottom, width, height;
	};

	class Collision final : public Singleton<Collision>
	{
	public:
		bool ParseLevelSVG(const std::string& file, const int yAdjust);
		bool ParseIntersectionsSVG(const std::string& file, const int yAdjust);
		
		void AddCollider(const Collider* object, const Rectf& collider);
		void RemoveCollider(const Collider* object);

		void UpdateCollider(const Collider* object, const Rectf& collider);

		std::vector<const Collider*> IsColliding(Collider* object);
		bool IsCollidingWithWorld(const Rectf& shape);
		bool IsCollidingWithWorld(const glm::vec2& point1, const glm::vec2& point2);
		bool IsCollidingWithIntersection(const Rectf& shape);

		//temp
		std::vector<std::vector<glm::vec2>> GetLevelCollider() const { return m_LevelCollider; }

	private:
		std::map<const Collider*, Rectf> m_Colliders;
		std::vector<std::vector<glm::vec2>> m_LevelCollider;
		std::vector<std::vector<glm::vec2>> m_Intersections;

		//Also taken from Prog2 Engine credits to Koen Samyn
		bool Raycast(const std::vector<glm::vec2>& vertices, const glm::vec2& rayP1, const glm::vec2& rayP2) const;
		bool Raycast(const glm::vec2* vertices, const size_t nrVertices, const glm::vec2& rayP1, const glm::vec2& rayP2) const;
		bool IntersectLineSegments(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2, float& outLambda1, float& outLambda2, float epsilon = 1e-6) const;
		bool IsPointOnLineSegment(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b) const;
		bool AreRectsColliding(const Rectf& rect1, const Rectf& rect2) const;
		
		float CrossProduct(const glm::vec2& vec1, const glm::vec2& vec2) const { return vec1.x * vec2.y - vec1.y * vec2.x; }
		float DotProduct(const glm::vec2& vec1, const glm::vec2& vec2) const { return vec1.x * vec2.x + vec1.y * vec2.y; }

		glm::vec2 CreateVector(const glm::vec2& fromPoint, const glm::vec2& tillPoint) const { return glm::vec2{ tillPoint.x - fromPoint.x, tillPoint.y - fromPoint.y }; }
	};
}

