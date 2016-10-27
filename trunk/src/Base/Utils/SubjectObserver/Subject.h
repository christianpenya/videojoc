#ifndef _BASE_SUBJECT_LUIS_RUIZ_1309161850_H
#define _BASE_SUBJECT_LUIS_RUIZ_1309161850_H

#include <vector>
#include <memory>

namespace unit_test {
	class SubjectObserverTest_registerTest_Test;
	class SubjectObserverTest_unregisterTest_Test;
	class SubjectObserverTest_registerTwiceTest_Test;
}

namespace base{ namespace utils{ namespace patterns {

	class Observer;

	class Subject
	{
		friend unit_test::SubjectObserverTest_registerTest_Test;
		friend unit_test::SubjectObserverTest_unregisterTest_Test;
		friend unit_test::SubjectObserverTest_registerTwiceTest_Test;
	public:
		Subject();
		Subject(const Subject &);

		virtual ~Subject();

		void registerObserver( Observer* );
		void unregisterObserver( Observer* );

		class NotificationBlocker
		{
		public:
			NotificationBlocker( Subject* );
			~NotificationBlocker();
		private:
			bool		m_previousState;
			Subject*	m_subject;
		};

	protected:
		void notifyObservers() const;

	private:
		friend NotificationBlocker;
		bool							m_notificationsEnabled;

		typedef std::vector<Observer*>	ObserversContainerType;
		ObserversContainerType			m_observers;
	};
}}}

#endif //_BASE_SUBJECT_LUIS_RUIZ_1309161850_H