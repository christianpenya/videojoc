#include "MaterialManager.h"

namespace engine {

	namespace materials {
		
		CMaterialManager::CMaterialManager(void) {
			m_path = "C:\\Users\\christian\\Documents\\VideojuegoTFM\\trunk\\bin\\Debug\\Videogame\\data\\materials\\materials.xml";
		}

		CMaterialManager::CMaterialManager(const std::string path) {
			m_path = path;
		}

		void CMaterialManager::LoadXML() {
			
			bool ok = true;
			
			tinyxml2::XMLDocument doc;
			
			tinyxml2::XMLError eResult = doc.LoadFile(m_path.c_str());
			if (eResult != tinyxml2::XML_SUCCESS) {
				ok = false;
			}

			tinyxml2::XMLNode* root = nullptr;
			if (ok) {
				root = doc.FirstChildElement("materials");
				if (root == nullptr) {
					ok = false;
				}
			}

			tinyxml2::XMLElement* element = nullptr;
			if (ok) {
				element = root->FirstChildElement("material");
				if (element == nullptr) {
					ok = false;
				}
			}
			
			printf(element->GetText());
		}

		/*MaterialManager::LoadXML(){
			xml = element.loadFile(m_path);
			el = xml.fce("materials");
			el2 = el.fce("material");
			while (el2){
				Material* mat = new Material(el2);
				std::string name = mat.getName();
				Add(name, mat);
				el2 = el.nse();
			}
		};*/
	}

}
