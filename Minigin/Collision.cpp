#include "Collision.h"
#include "Collider.h"

#include "GameObject.h"
bool diji::Collision::ParseLevelSVG(const std::string& file, const int yAdjust)
{
	return SVGParser::GetVerticesFromSvgFile(file, m_LevelCollider, yAdjust);
}

void diji::Collision::AddCollider(const Collider* object, const Rectf& collider)
{
	//const auto colliders = std::vector<RectI>(1, collider);
	m_Colliders[object] = collider;
}

void diji::Collision::RemoveCollider(const Collider* object)
{
	auto it = m_Colliders.find(object);
	if (it != m_Colliders.end())
		m_Colliders.erase(it);
}

void diji::Collision::UpdateCollider(const Collider* object, const Rectf& collider)
{
	//const auto colliders = std::vector<RectI>(1, collider);
	m_Colliders[object] = collider;
}

std::vector<const diji::Collider*> diji::Collision::IsColliding(Collider* object)
{
	std::vector<const Collider*> collidingObjects;
	for (auto& pair : m_Colliders)
	{
		if (pair.first != object)
		{
			if (AreRectsColliding(m_Colliders[object], pair.second))
			{
				collidingObjects.push_back(pair.first);
			}
		}
	}
	return collidingObjects;
}

bool diji::Collision::AreRectsColliding(const Rectf& rect1, const Rectf& rect2) const
{
	if ((rect1.left + rect1.width) < rect2.left || (rect2.left + rect2.width) < rect1.left)
 		return false;

	if (rect1.bottom > (rect2.bottom + rect2.height) || rect2.bottom > (rect1.bottom + rect1.height))
		return false;

	return true;
}

bool diji::Collision::IsCollidingWithWorld(const Rectf& shape)
{
	//try with only two raycast up down and left right
	const auto playerShape = shape;
	for (const auto& collisionBox : m_LevelCollider)
	{
		const glm::vec2 bottomLeft(playerShape.left, playerShape.bottom);
		const glm::vec2 topLeft(playerShape.left, playerShape.bottom + playerShape.height);

		if (Raycast(collisionBox, bottomLeft, topLeft))
			return true;

		const glm::vec2 topRight(playerShape.left + playerShape.width, playerShape.bottom + playerShape.height);
		if (Raycast(collisionBox, topLeft, topRight))
			return true;

		const glm::vec2 bottomRight(playerShape.left + playerShape.width, playerShape.bottom);
		if (Raycast(collisionBox, topRight, bottomRight))
			return true;

		if (Raycast(collisionBox, bottomRight, bottomLeft))
			return true;
	}
	return false;
}

#pragma region utility functions
bool diji::Collision::Raycast(const std::vector<glm::vec2>& vertices, const glm::vec2& rayP1, const glm::vec2& rayP2) const
{
	return Raycast(vertices.data(), vertices.size(), rayP1, rayP2);
}

bool diji::Collision::Raycast(const glm::vec2* vertices, const size_t nrVertices, const glm::vec2& rayP1, const glm::vec2& rayP2) const
{
	if (nrVertices == 0)
		return false;

	Rectf r1{}, r2{};
	// r1: minimal AABB rect enclosing the ray
	r1.left = std::min(rayP1.x, rayP2.x);
	r1.bottom = std::min(rayP1.y, rayP2.y);
	r1.width = std::max(rayP1.x, rayP2.x) - r1.left;
	r1.height = std::max(rayP1.y, rayP2.y) - r1.bottom;

	// Line-line intersections.
	for (size_t idx{ 0 }; idx <= nrVertices; ++idx)
	{
		// Consider line segment between 2 consecutive vertices
		// (modulo to allow closed polygon, last - first vertice)
		const glm::vec2 q1 = vertices[(idx + 0) % nrVertices];
		const glm::vec2 q2 = vertices[(idx + 1) % nrVertices];

		// r2: minimal AABB rect enclosing the 2 vertices
		r2.left = std::min(q1.x, q2.x);
		r2.bottom = std::min(q1.y, q2.y);
		r2.width = std::max(q1.x, q2.x) - r2.left;
		r2.height = std::max(q1.y, q2.y) - r2.bottom;

		if (AreRectsColliding(r1, r2))
		{
			float lambda1{};
			float lambda2{};
			if (IntersectLineSegments(rayP1, rayP2, q1, q2, lambda1, lambda2))
			{
				if (lambda1 > 0 && lambda1 <= 1 && lambda2 > 0 && lambda2 <= 1)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool diji::Collision::IntersectLineSegments(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2, float& outLambda1, float& outLambda2, float epsilon) const
{
	bool intersecting{ false };

	const glm::vec2 p1p2 = CreateVector(p1, p2);
	const glm::vec2 q1q2 = CreateVector(q1, q2);
	//const glm::vec2 q1q2{ q1, q2 };

	// Cross product to determine if parallel
	float denom = CrossProduct(p1p2, q1q2);

	// Don't divide by zero
	if (std::abs(denom) > epsilon)
	{
		intersecting = true;

		//const glm::vec2 p1q1{ p1, q1 };
		const glm::vec2 p1q1 = CreateVector(p1, q1);

		const float num1 = CrossProduct(p1q1, q1q2);
		const float num2 = CrossProduct(p1q1, p1p2);
		outLambda1 = num1 / denom;
		outLambda2 = num2 / denom;
	}
	else // are parallel
	{
		// Connect start points
		//const glm::vec2 p1q1{ p1, q1 };
		const glm::vec2 p1q1 = CreateVector(p1, q1);


		// Cross product to determine if segments and the line connecting their start points are parallel, 
		// if so, than they are on a line
		// if not, then there is no intersection
		if (std::abs(CrossProduct(p1q1, q1q2)) > epsilon)
		{
			return false;
		}

		// Check the 4 conditions
		outLambda1 = 0;
		outLambda2 = 0;
		if (IsPointOnLineSegment(p1, q1, q2) ||
			IsPointOnLineSegment(p2, q1, q2) ||
			IsPointOnLineSegment(q1, p1, p2) ||
			IsPointOnLineSegment(q2, p1, p2))
		{
			intersecting = true;
		}
	}
	return intersecting;
}

bool diji::Collision::IsPointOnLineSegment(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b) const
{
	const glm::vec2 ap = CreateVector(a, p);
	const glm::vec2 bp = CreateVector(b, p);

	// If not on same line, return false
	if (abs(CrossProduct(ap, bp)) > 0.001f)
	{
		return false;
	}

	// Both vectors must point in opposite directions if p is between a and b
	if (DotProduct(ap, bp) > 0)
	{
		return false;
	}

	return true;
}
#pragma endregion