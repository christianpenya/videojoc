#ifndef _BASE_SUBSCRIBER_LUIS_RUIZ_1509161243_H
#define _BASE_SUBSCRIBER_LUIS_RUIZ_1509161243_H

#include "Event.h"

#include <boost/function.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <map>

namespace boost { namespace signals2 {
	class scoped_connection;
}}

namespace base { namespace utils { namespace patterns {
	class Subscriber
	{
		typedef ::boost::function<void (const Event&)> CallbackType;
		typedef  boost::shared_ptr<boost::signals2::scoped_connection> ConnectionTypePtr;
		typedef  std::map<EventType, ConnectionTypePtr> SubscriptionsMapType;
	public:
		Subscriber();
		~Subscriber();
		void subscribe(CallbackType cb, const EventType& evtName);
		void unsubscribe(const EventType& evtName);
	private:
		SubscriptionsMapType m_subscriptions;
	};
}}}

#endif //_BASE_SUBSCRIBER_LUIS_RUIZ_1509161243_H