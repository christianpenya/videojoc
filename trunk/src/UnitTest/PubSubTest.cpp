#include <gtest/gtest.h>

#include "Base/Utils/PublisherSubscriber/Broker.h"
#include "Base/Utils/PublisherSubscriber/Subscriber.h"

namespace unit_test {

	struct MyEvent : public base::utils::patterns::Event
	{
		MyEvent()
		{ 
			
		}
	};

	class PubSubTest : public testing::Test
	{
	public:
		PubSubTest()
		{
		}

		void SetUp()
		{

		}

		void TearDown()
		{

		}

		void test()
		{

		}

	};

	TEST_F(PubSubTest, registerTest)
	{
		//base::utils::patterns::Broker::GetInstance().subscribe(boost::bind(&PubSubTest::test, this), MyEvent::m_name);
	}
}