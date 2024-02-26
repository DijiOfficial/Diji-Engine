#pragma once
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

#include <string>

//#include <chrono>
namespace diji 
{
	class GameObject;

	class Component
	{
	public:
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Update() = 0;

	protected:
		explicit Component(GameObject* ownerPtr) : m_OwnerPtr{ ownerPtr } {}
		GameObject* GetOwner() const { return m_OwnerPtr; }
		//std::shared_ptr<GameObject> GetOwner() const { return m_OwnerPtr; }
	private:
		GameObject* m_OwnerPtr{};
		//std::shared_ptr<GameObject> m_OwnerPtr{};
	};
}
