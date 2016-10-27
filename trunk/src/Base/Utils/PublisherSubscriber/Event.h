#ifndef _BASE_EVENT_LUIS_RUIZ_1509161243_H
#define _BASE_EVENT_LUIS_RUIZ_1509161243_H

#include <string>

namespace base { namespace utils { namespace patterns {

	typedef std::string EventType;

	struct Event
	{
		virtual ~Event(){;}
		EventType m_name;
	};
}}}

#endif //_BASE_EVENT_LUIS_RUIZ_1509161243_H