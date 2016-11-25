#ifndef _BASE_TEMPLATEDMAP_LR_2010201620428_H
#define _BASE_TEMPLATEDMAP_LR_2010201620428_H

#include <map>
#include <string>
#include "CheckedDelete.h"

namespace base
{
	namespace utils
	{
		
		class Element
		{
		};

		template <class T>
		class CTemplatedMap
		{
		public:
			typedef std::map<std::string, T*> TMapResources;

		public:
			CTemplatedMap(){}
			virtual ~CTemplatedMap()
			{
				Destroy();
			}

			bool Exist(const std::string &aName)
			{
				return m_ResourcesMap.find(aName) != m_ResourcesMap.end();
			}

			void Remove(const std::string &aName)
			{
				m_ResourcesMap.erase(aName);
			}

			virtual T * operator()(const std::string &aName)
			{
				return m_ResourcesMap[aName];
			}

			virtual bool Add(const std::string &aName, T *Resource)
			{
				bool lOk = false;
				if (m_ResourcesMap.find(aName) == m_ResourcesMap.end())
				{
					m_ResourcesMap.insert(std::make_pair(aName, Resource));
					lOk = true;
				}
				return lOk;
			}

			virtual void Update(const std::string &aName, T *aResource)
			{
				TMapResources::iterator lItFind = m_ResourcesMap.find(aName);
				if (lItFind != m_ResourcesMap.end())
				{
					base::utils::CheckedDelete(lItFind->second);
				}
				m_ResourcesMap[aName] = aResource;
			}

			virtual void Clear()
			{
				m_ResourcesMap.clear();
			}

			virtual void Destroy()
			{
				for (TMapResources::iterator itb = m_ResourcesMap.begin(), ite = m_ResourcesMap.end(); itb != ite; ++itb)
					base::utils::CheckedDelete(itb->second);

				Clear();
			}

			TMapResources & GetResourcesMap()
			{
				return m_ResourcesMap;
			}

		protected:
			TMapResources	 m_ResourcesMap;
		};
	}
}

#endif	// _BASE_TEMPLATEDMAP_LR_2010201620428_H
