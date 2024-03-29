#pragma once

namespace diji 
{
	enum class EventNames
	{
		Death,
		Health
	};

	class ISubject;
	class GameObject;
	class Text;

	class IObserver
	{
	public:
		//IObserver(EventNames& event) : m_Event{ event } {};
		IObserver(GameObject* subject) : m_Event{ EventNames::Health }, m_Subject{ subject } {};
		virtual ~IObserver() = default;

		//I pass the owner from the comp but need the comp anyways, will change next week as I am behind on schedule rn
		virtual void OnNotify(const GameObject* entity) = 0;

		IObserver(const IObserver& other) = delete;
		IObserver(IObserver&& other) = delete;
		IObserver& operator=(const IObserver& other) = delete;
		IObserver& operator=(IObserver&& other) = delete;
	
	protected:
		GameObject* m_Subject;
	private:
		EventNames m_Event;
	};

	class HealthObserver final : public IObserver
	{
		public:
			explicit HealthObserver(GameObject* gameObj, GameObject* subject);

			void OnNotify(const GameObject* entity) override;
		
		private:
			Text* m_TextCompPtr;

	};

	class ScoreObserver final : public IObserver
	{
	public:
		explicit ScoreObserver(GameObject* observer, GameObject* subject);

		void OnNotify(const GameObject* entity) override;

	private:
		Text* m_TextCompPtr;

	};
}