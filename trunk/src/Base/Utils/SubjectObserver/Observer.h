#ifndef _BASE_OBSERVER_LUIS_RUIZ_1309161900_H
#define _BASE_OBSERVER_LUIS_RUIZ_1309161900_H

namespace base { namespace utils{ namespace patterns {
	class Subject;

	class Observer
	{
	public:
		virtual void update(const Subject*) = 0;
	};
}}}

#endif //_BASE_OBSERVER_LUIS_RUIZ_1309161900_H