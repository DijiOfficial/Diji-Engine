#pragma once
#include "IObserver.h"

#include <forward_list>
#include <unordered_map>

namespace diji
{
	enum class MessageTypes
	{
		PLAYSOUND, 
		HANDLEPHYSICS,
		LOG
	};

	class ISubject
	{
	public:
		ISubject() = default;
		virtual ~ISubject() = default;

		ISubject(const ISubject& other) = delete;
		ISubject(ISubject&& other) = delete;
		ISubject& operator=(const ISubject& other) = delete;
		ISubject& operator=(ISubject&& other) = delete;

		virtual void AddObserver(MessageTypes message, IObserver* observer);
		virtual void RemoveObserver(MessageTypes message, IObserver* observer);
		virtual void NotifyAll();
		virtual void Notify(MessageTypes message);
	
	private:
		typedef std::forward_list<IObserver*> ObserversList;
		typedef std::unordered_map<MessageTypes, ObserversList> ObserversMap;

		ObserversMap m_Observers;
	};
}


