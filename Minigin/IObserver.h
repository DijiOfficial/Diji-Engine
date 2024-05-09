#pragma once

namespace diji 
{
	enum class MessageTypes { };

	class Subject;

	class IObserver
	{
	public:
		IObserver() = default;
		virtual ~IObserver() = default;

		IObserver(const IObserver& other) = delete;
		IObserver(IObserver&& other) = delete;
		IObserver& operator=(const IObserver& other) = delete;
		IObserver& operator=(IObserver&& other) = delete;

		virtual void OnNotify(MessageTypes message, Subject* subject) = 0;
	};
}