#pragma once
#include "Component.h"

#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace diji
{
	class Transform;
	class Render;

	class GameObject final
	{
	public:
		GameObject() = default;
		~GameObject() = default;

		template<typename... Args>
		GameObject(Args&&... args) { AddComponents(std::forward<Args>(args)...); }; //untested

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update();
		void FixedUpdate();
		void Init();
		void Render() const;

#pragma region Components
		template<typename T, typename... Args>
		void AddComponents(Args&&... args)
		{
			static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

			m_ComponentsPtrVec.push_back(std::make_unique<T>(this, std::forward<Args>(args)...));

			if constexpr (std::is_same_v<T, Transform>)
			{
				m_TransformCompPtr = dynamic_cast<Transform*>(m_ComponentsPtrVec.back().get());
				m_LocalPosition = GetWorldPosition();
			}
			else if constexpr (std::is_same_v<T, diji::Render>)
			{
				m_RenderCompPtr = dynamic_cast<diji::Render*>(m_ComponentsPtrVec.back().get());
			}
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
#pragma endregion

		//SceneGraph
		GameObject* GetParent() const { return m_ParentPtr; };
		GameObject* GetChild(int index) const { return m_ChildrenPtrVec[index]; };
		int GetChildCount() const { return static_cast<int>(m_ChildrenPtrVec.size()); };
		const glm::vec3& GetWorldPosition();

		void SetParent(GameObject* parent, bool keepWorldPosition);
		void SetLocalPosition(const glm::vec3& pos);

	private:
		bool m_PositionIsDirty{ false };
		glm::vec3 m_LocalPosition{ 0 ,0 ,0 };
		GameObject* m_ParentPtr{ nullptr };
		Transform* m_TransformCompPtr{ nullptr };
		diji::Render* m_RenderCompPtr{ nullptr };
		std::vector<std::unique_ptr<Component>> m_ComponentsPtrVec{};
		std::vector<GameObject*> m_ChildrenPtrVec{};

		bool IsChildOf(GameObject* potentialChild) const;
		void SetPositionDirty();
		void UpdateWorldPosition();
		
		void AddChild(GameObject* child);
		void RemoveChild(GameObject* child);
	};
}