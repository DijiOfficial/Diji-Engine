#pragma once
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"

#include <vector>
#include <memory>
#include <string>

//#include <chrono>
namespace diji 
{
	class GameObject;

	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		Component(const Component& other) = default;
		Component(Component&& other) = default;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Update(GameObject& gameObject) = 0;
		virtual void Render(const GameObject& gameObject) const = 0;
	};
}
