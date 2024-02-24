#pragma once

//probably make a header file with all the includes for the components
#include "FPSCounter.h"
#include "Texture.h"
#include <type_traits>

// todo: this should become final.
class GameObject 
{
public:
	GameObject() = default;
	virtual ~GameObject();

	template<typename... Args>
	GameObject(Args&&... args) { AddComponents(std::forward<Args>(args)...); }; //untested

	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;
		
	virtual void Update();
	virtual void Render() const;
	
	template<typename T, typename... Args>
	void AddComponents(Args&&... args)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

		m_ComponentsPtrVec.push_back(std::make_unique<T>(std::forward<Args>(args)...));
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
	void SetTexture(const std::string& filename);
	void SetPosition(float x, float y);


	void SetNoTexture() { hasTexture = false; }
	bool GetHasTexture() const { return hasTexture; }
private:
	Transform m_transform{};
	// todo: mmm, every gameobject has a texture? Is that correct?
	std::shared_ptr<Texture2D> m_texture{};
	std::vector<std::unique_ptr<Component>> m_ComponentsPtrVec{};
	bool hasTexture = true;
};
