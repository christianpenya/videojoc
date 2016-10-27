#include "Broker.h"

#include "Event.h"

#include <boost/signals2/signal.hpp>
#include <boost/thread/mutex.hpp>

namespace base { namespace utils { namespace patterns {

	Broker::Broker()
	: m_subscriptions()
	,m_mutexSubscriptions(new boost::mutex)
	{

	}
	Broker::~Broker()
	{
		boost::mutex::scoped_lock lock(*m_mutexSubscriptions);

		TStrSubscriberMap::iterator it = m_subscriptions.begin();
		while(it != m_subscriptions.end())
		{
			it->second.reset();
			++it;
		}
		m_subscriptions.clear();
	}

	base::utils::patterns::Broker::SignalTypePtr Broker::getSignal(const EventType& evtName)
	{
		SignalTypePtr sig;
		
		boost::mutex::scoped_lock lock(*m_mutexSubscriptions);
		
		TStrSubscriberMap::iterator it = m_subscriptions.find(evtName);

		if(it == m_subscriptions.end())
		{
			sig = SignalTypePtr(new SignalType());
			m_subscriptions[evtName] =  sig;
		}
		else
		{
			sig = it->second;
		}
		
		return sig;
	}

	base::utils::patterns::Broker::ConnectionTypePtr Broker::subscribe(CallbackType cb, const EventType& evtName)
	{
		SignalTypePtr sig = getSignal(evtName);
		return ConnectionTypePtr(new ConnectionType(sig.get()->connect(cb)));
	}

	void Broker::publish(const Event& evt)
	{
		SignalTypePtr sig = getSignal(evt.m_name);
		(*sig)(evt);
	}
}}}