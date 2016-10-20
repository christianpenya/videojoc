#ifndef _BASE_TEMPLATEDMAP_CPB_20102016204214_H
#define _BASE_TEMPLATEDMAP_CPB_20102016204214_H

#include <map>
#include <string>

namespace base {

	namespace utils {

		class CTemplatedMap {

		private:
			std::map<std::string, unsigned int> m_ResourcesMap;

			CTemplatedMap();
			~CTemplatedMap();

			bool Exist(std::string key) const {
				
				bool found = false;
				std::map<std::string, unsigned int>::iterator it;

				if (m_ResourcesMap.find(key) != m_ResourcesMap.end()) {
					found = true;
				}
				else {
					found = false;
				}
			
				return found;
			}

			void Add(std::string key, unsigned int element) {

				m_ResourcesMap.insert(std::make_pair(key, element));
			}

			void Remove(std::string key) {

				if (Exist(key)) {
					m_ResourcesMap.erase(key);
				}
			}

			void Update(const std::string& key, unsigned int newElement) {

				if (Exist(key)) {
					m_ResourcesMap[key] = newElement;
				}
				else {
					Add(key, newElement);
				}
			}

			unsigned int get(const std::string& key) {

				return (*m_ResourcesMap.find{ key }.second);
			}

		};
	}
}

#endif	// _BASE_TEMPLATEDMAP_CPB_20102016204214_H