#pragma once

namespace diji 
{
	class ISubject;

	class IObserver
	{
	public:
		IObserver() = default;
		virtual ~IObserver() = default;

		virtual void OnNotify() = 0;

		IObserver(const IObserver& other) = delete;
		IObserver(IObserver&& other) = delete;
		IObserver& operator=(const IObserver& other) = delete;
		IObserver& operator=(IObserver&& other) = delete;
	};

	class DeathEvent final : public IObserver
	{
	public:
		explicit DeathEvent() = default;

		void OnNotify() override 
		{
			m_HasDied = true;
		}; 

	private:
		bool m_HasDied{ false };

	};
}