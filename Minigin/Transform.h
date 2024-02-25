#pragma once
#include <glm/glm.hpp>
#include "Component.h"

namespace diji
{
	class GameObject;

	class Transform final : public Component
	{
	public:
		Transform() = default;
		Transform(const float x, const float y, const float z) { SetPosition(x, y, z); };
		Transform(const float x, const float y) { SetPosition(x, y); };
		Transform(int x, int y, int z) { SetPosition(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)); };
		Transform(int x, int y) { SetPosition(static_cast<float>(x), static_cast<float>(y)); };
		virtual ~Transform() = default;


		Transform(const Transform& other) = default;
		Transform(Transform&& other) = default;
		Transform& operator=(const Transform& other) = delete;
		Transform& operator=(Transform&& other) = delete;

		virtual void Update(GameObject& gameObject) override { (void)gameObject; };
		virtual void Render(const GameObject& gameObject) const override { (void)gameObject; };

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const float x, const float y, const float z);
		void SetPosition(const float x, const float y);

	private:
		glm::vec3 m_Position;
	};
}
