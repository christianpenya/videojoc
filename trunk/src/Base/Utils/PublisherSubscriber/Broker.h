#ifndef _BASE_BROKER_LUIS_RUIZ_1509161243_H
#define _BASE_BROKER_LUIS_RUIZ_1509161243_H

#define _SCL_SECURE_NO_WARNINGS

#include "../Singleton.h"
#include "Event.h"

#include <boost/function.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <boost/signals2/signal.hpp>

#include <map>

namespace boost {
	class mutex;
}

namespace base { namespace utils { namespace patterns {
	class Subscriber;
	struct Event;

	class Broker : public CSingleton<Broker>
	{
		typedef  boost::signals2::scoped_connection ConnectionType;
		typedef  boost::shared_ptr<ConnectionType> ConnectionTypePtr;
		typedef boost::function<void (const Event&)> CallbackType;
		typedef boost::signals2::signal<void (const Event&)> SignalType;
		typedef boost::shared_ptr<SignalType> SignalTypePtr;

	public:
		Broker();
		virtual ~Broker();
		Broker(const Broker &);
		Broker & operator=(const Broker&);
		Broker * operator&();
	private:
		SignalTypePtr getSignal(const EventType& evtName);
	public:
		void unsubscribe(CallbackType cb, const EventType& evtName);
		ConnectionTypePtr subscribe(CallbackType cb, const EventType& evtName);
		void publish(const Event& evt);
	
	private:
		typedef std::map<EventType, SignalTypePtr > TStrSubscriberMap;

		TStrSubscriberMap m_subscriptions;
		boost::shared_ptr<boost::mutex> m_mutexSubscriptions;
	};
}}}
#endif //_BASE_BROKER_LUIS_RUIZ_1509161243_H