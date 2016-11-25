#pragma once

#ifndef _ENGINE_MATERIALMANAGER_CPB_1611241620428_H
#define _ENGINE_MATERIALMANAGER_CPB_1611241620428_H


#include "Base/XML/tinyxml2/tinyxml2.h"
#include <list>
#include "Base\Utils\TemplatedMap.h"
#include "Base\Utils\Name.h"
#include "Material.h"

namespace engine {
	namespace materials {
		
		class CMaterialManager : public base::utils::CTemplatedMap<CMaterial> {
		
		public:
			CMaterialManager(void);
			CMaterialManager(const std::string path);
			~CMaterialManager() {};
			void LoadXML();
			void ReloadXML();

		private:
			std::list<CMaterial> materialList;
			std::string m_path;

		};		
	}
}

#endif //_ENGINE_MATERIALMANAGER_CPB_1611241620428_H