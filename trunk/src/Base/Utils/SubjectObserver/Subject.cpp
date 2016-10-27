#include "Subject.h"

#include "Observer.h"

#include <algorithm>

namespace base{ namespace utils{ namespace patterns {

	Subject::NotificationBlocker::NotificationBlocker(Subject* subject)
		:m_subject(subject)
	{
		m_previousState = m_subject->m_notificationsEnabled;
		m_subject->m_notificationsEnabled = false;
	}

	Subject::NotificationBlocker::~NotificationBlocker()
	{
		m_subject->m_notificationsEnabled = m_previousState;
	}

	Subject::Subject() :
	m_notificationsEnabled(true),
	m_observers()
	{
	}

	Subject::Subject(const Subject & rhs) : 
	m_notificationsEnabled( true ),
	m_observers( rhs.m_observers )
	{

	}

	Subject::~Subject()
	{
	}

	void Subject::registerObserver( Observer* observer )
	{
		ObserversContainerType::iterator it;
		it = std::find(m_observers.begin(), m_observers.end(), observer);

		if (it == m_observers.end())
		{
			m_observers.push_back(observer);
		}
	}

	void Subject::unregisterObserver( Observer* observer )
	{
		ObserversContainerType::iterator it;
		it = std::find( m_observers.begin(), m_observers.end(), observer);

		if ( it != m_observers.end() )
		{
			m_observers.erase(it);
		}
	}

	void Subject::notifyObservers() const
	{
		if ( m_notificationsEnabled )
		{
			for ( ObserversContainerType::const_iterator it = m_observers.begin(); 
				  it != m_observers.end(); 
				  ++it )
			{
				(*it)->update(this);
			}
		}
	}
}}}
