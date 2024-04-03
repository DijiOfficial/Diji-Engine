#pragma once
#include "IObserver.h"
#include "Singleton.h"

#include <forward_list>
#include <unordered_map>
#include <memory>


namespace diji
{
	class Subject
	{
	public:
		Subject() = default;
		virtual ~Subject() = default;

		Subject(const Subject& other) = default;
		Subject(Subject&& other) = default;
		Subject& operator=(const Subject& other) = delete;
		Subject& operator=(Subject&& other) = delete;

		void AddObserver(MessageTypes message, IObserver* observer);
		void RemoveObserver(MessageTypes message, IObserver* observer);
	
	protected:
		void Notify(MessageTypes message);

	private:	
		typedef std::forward_list<IObserver*> ObserverPtrList;
		typedef std::unordered_map<MessageTypes, ObserverPtrList> ObserversMap;

		ObserversMap m_ObserversMap;
	};
}


