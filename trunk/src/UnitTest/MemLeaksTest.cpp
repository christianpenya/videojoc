#include <gtest/gtest.h>

#include "Base/Utils/MemLeaks/MemLeaks.h"

namespace unit_test {
	class Element
	{
	};

	TEST(MemLeaksTestAdd)
	{
		MemLeaks::MemoryBegin();

		{
			std::auto_ptr<Element> element(new Element);
		}

		MemLeaks::MemoryEnd();
	}

}