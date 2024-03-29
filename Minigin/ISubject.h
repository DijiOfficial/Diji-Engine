#pragma once
#include "IObserver.h"
#include "Singleton.h"

#include <forward_list>
#include <unordered_map>
#include <memory>


namespace diji
{
	enum class MessageTypes
	{
		ENTITY_COLLISION, 
		SCORE_UPDATE,
		LOG
	};

	class ISubject : public Singleton<ISubject>
	{
	public:
		ISubject() = default;
		virtual ~ISubject() = default;

		ISubject(const ISubject& other) = default;
		ISubject(ISubject&& other) = default;
		ISubject& operator=(const ISubject& other) = delete;
		ISubject& operator=(ISubject&& other) = delete;

		//remove virtual
		virtual void AddObserver(MessageTypes message, std::unique_ptr<IObserver>&& observer);
		virtual void RemoveObserver(MessageTypes message, IObserver* observer);
		virtual void NotifyAll(const GameObject* entity);
		virtual void Notify(const GameObject* entity, MessageTypes message);
	
	private:
		typedef std::forward_list<std::unique_ptr<IObserver>> ObserversList;
		typedef std::unordered_map<MessageTypes, ObserversList> ObserversMap;

		ObserversMap m_Observers;
	};

	//class PlayerSubject final : public ISubject
	//{
	//	public:
	//		MessageTypes m_Message;
	//};
}


