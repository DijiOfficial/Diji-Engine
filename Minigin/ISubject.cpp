#include "ISubject.h"

// Message is the 'key' or 'event' that we want to handle.
// 'observer' is the observer we'll add to the events
void diji::ISubject::AddObserver(MessageTypes message, std::unique_ptr<IObserver>&& observer)
{
	// Check to see if our 'key' or 'event type' is in our unordered_map
	auto it = m_Observers.find(message);
	if (it == m_Observers.end())
	{
		// Since we did not find our message, we need
		// to construct a new list
		m_Observers[message] = ObserversList();
	}
	// Add our observer to the appropriate 'bucket' of events.
	m_Observers[message].push_front(std::move(observer));
}

void diji::ISubject::RemoveObserver(MessageTypes message, IObserver* observer)
{
	// Check to see if our 'key' or 'event type' is in our unordered_map
	auto it = m_Observers.find(message);
	// Remove it
	if (it != m_Observers.end())
	{
		auto& list = it->second;
		list.remove_if([observer](const std::unique_ptr<IObserver>& ptr)
			{
				return ptr.get() == observer;
			});
	}
}

void diji::ISubject::NotifyAll(const GameObject* entity)
{
	// Search through every message type (our keys)
	for (auto it = m_Observers.begin(); it != m_Observers.end(); ++it)
		for (auto& observer : m_Observers[it->first])
			observer->OnNotify(entity);
}

void diji::ISubject::Notify(const GameObject* entity, MessageTypes message)
{
	for (auto& observer : m_Observers[message])
		observer->OnNotify(entity);
}