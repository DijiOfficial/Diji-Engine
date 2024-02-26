#pragma once
#include <vector>
#include <memory>
#include "Component.h"
namespace diji
{
	class Component;

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

	private:
		std::vector<std::unique_ptr<Component>> m_ComponentsPtrVec{};
	};

}