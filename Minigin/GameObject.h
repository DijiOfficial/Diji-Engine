#pragma once
#include "Component.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace diji
{
	class Component;
	class Transform;
	class Render;

	class GameObject final
	{
	public:
		GameObject() = default;
		~GameObject();

		template<typename... Args>
		GameObject(Args&&... args) { AddComponents(std::forward<Args>(args)...); }; //untested

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update();
		void Render() const;

		template<typename T, typename... Args>
		void AddComponents(Args&&... args)
		{
			static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

			m_ComponentsPtrVec.push_back(std::make_unique<T>(this, std::forward<Args>(args)...));
		}

		void RemoveComponent(const Component& component);
		const std::vector<std::unique_ptr<Component>>& GetComponents() const { return m_ComponentsPtrVec; };

		template<typename T>
		T* GetComponent() const
		{
			static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

			for (const auto& comp : m_ComponentsPtrVec)
			{
				if (dynamic_cast<T*>(comp.get()) != nullptr)
				{
					return static_cast<T*>(comp.get());
				}
			}
			return nullptr;
		}

		template<typename T>
		bool HasComponent() const
		{
			static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

			for (const auto& comp : m_ComponentsPtrVec)
			{
				if (dynamic_cast<T*>(comp.get()) != nullptr)
				{
					return true;
				}
			}
			return false;
		}

		//SceneGraph
		GameObject* GetParent() const { return m_ParentPtr; };
		GameObject* GetChild(int index) const { return m_ChildrenPtrVec[index]; };
		int GetChildCount() const { return static_cast<int>(m_ChildrenPtrVec.size()); };
		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; };

		void SetParent(GameObject* parent, bool keepWorldPosition);
		void AddChild(GameObject* child);
		void RemoveChild(GameObject* child);
		void SetLocalPosition(const glm::vec3& pos);

		void SetPositionClean() { m_PositionIsDirty = false; };
		const glm::vec3& GetWorldPosition();
	private:
		bool m_PositionIsDirty{ false };
		glm::vec3 m_LocalPosition{ 0 ,0 ,0 };
		GameObject* m_ParentPtr{};
		Transform* m_TransformCompPtr{ nullptr };
		diji::Render* m_RenderCompPtr{ nullptr };
		std::vector<std::unique_ptr<Component>> m_ComponentsPtrVec{};
		std::vector<GameObject*> m_ChildrenPtrVec{};

		bool IsChildOf(GameObject* potentialChild) const;
		void SetPositionDirty();
		void UpdateWorldPosition();
	};

}