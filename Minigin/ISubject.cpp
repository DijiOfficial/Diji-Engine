#include "ISubject.h"

// Message is the 'key' or 'event' that we want to handle.
// 'observer' is the observer we'll add to the events
void diji::ISubject::AddObserver(MessageTypes message, IObserver* observer)
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
	m_Observers[message].push_front(observer);
}

void diji::ISubject::RemoveObserver(MessageTypes message, IObserver* observer)
{
	// Check to see if our 'key' or 'event type' is in our unordered_map
	auto it = m_Observers.find(message);
	// Remove it
	if (it != m_Observers.end())
	{
		auto& list = it->second;
		list.remove(observer);
	}
}

void diji::ISubject::NotifyAll()
{
	// Search through every message type (our keys)
	for (auto it = m_Observers.begin(); it != m_Observers.end(); ++it)
		for (auto& observer : m_Observers[it->first])
			observer->OnNotify();
}

void diji::ISubject::Notify(MessageTypes message)
{
	for (auto& observer : m_Observers[message])
		observer->OnNotify();
}