#include <gtest/gtest.h>
#include "Base/Utils/TemplatedMap.h"

namespace unit_test{
	class Element{

	};

	TEST(TestTemplateMap1){
		base::utils::CTemplatedMap<Element> m_elementsContainer;
		ASSERT_EQ(0, m_elementsContainer.GetResourcesMap().size());
		m_elementsContainer.Add("Element1", new Element);
		ASSERT_EQ(1, m_elementsContainer.GetResourcesMap().size());
	}
	
}
