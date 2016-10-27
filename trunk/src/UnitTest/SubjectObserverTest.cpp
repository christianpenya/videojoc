#include <gtest/gtest.h>

#include "Base/Utils/SubjectObserver/Observer.h"
#include "Base/Utils/SubjectObserver/Subject.h"

namespace unit_test {

	using namespace base::utils::patterns;

	class Observer1 : public Observer
	{
	public:
		Observer1() : m_subjectUpdated(NULL){ ; }
		~Observer1(){ ; }
		const Subject* m_subjectUpdated;

		void update(const Subject* subject)
		{
			m_subjectUpdated = subject;
		}
	};

	class SubjectObserverTest : public testing::Test
	{
	public:
		SubjectObserverTest()
		{
		}

		void SetUp()
		{

		}

		void TearDown()
		{

		}

		Subject m_subject;
		Observer1 m_observer;
	};

	TEST_F(SubjectObserverTest, registerTest)
	{
		ASSERT_EQ(0, m_subject.m_observers.size());

		m_subject.registerObserver(&m_observer);

		ASSERT_EQ(1, m_subject.m_observers.size());

		ASSERT_EQ(NULL, m_observer.m_subjectUpdated);

		m_subject.notifyObservers();

		ASSERT_EQ(&m_subject, m_observer.m_subjectUpdated);
	}

	TEST_F(SubjectObserverTest, unregisterTest)
	{
		m_subject.registerObserver(&m_observer);

		ASSERT_EQ(1, m_subject.m_observers.size());

		m_subject.unregisterObserver(&m_observer);

		ASSERT_EQ(0, m_subject.m_observers.size());

		m_subject.notifyObservers();

		ASSERT_EQ(NULL, m_observer.m_subjectUpdated);
	}

	TEST_F(SubjectObserverTest, registerTwiceTest)
	{
		m_subject.registerObserver(&m_observer);

		ASSERT_EQ(1, m_subject.m_observers.size());

		m_subject.registerObserver(&m_observer);

		ASSERT_EQ(1, m_subject.m_observers.size());

		m_subject.notifyObservers();

		ASSERT_EQ(&m_subject, m_observer.m_subjectUpdated);
	}
}