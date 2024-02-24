#pragma once
#include "Transform.h"
//not sure about those two
#include "ResourceManager.h"
#include "Renderer.h"

#include <vector>
#include <memory>
#include <string>

//#include <chrono>
//#include <iostream>
class Component
{
public:
	Component() = default;
	virtual ~Component() = default;

	Component(const Component& other) = default;
	Component(Component&& other) = default;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) = delete;

	virtual void Update();
	//virtual void Update(std::vector<std::unique_ptr<Component>>& compVec);
	virtual void Render() const;

private:

};
