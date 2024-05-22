#pragma once
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

		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void FixedUpdate() = 0;

	protected:
		explicit Component(GameObject* ownerPtr) : m_OwnerPtr{ ownerPtr } {}
		const GameObject* GetOwner() const { return m_OwnerPtr; }

	private:
		GameObject* m_OwnerPtr{};
	};
}
