#pragma once

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
	virtual void Render() const;

private:

};
