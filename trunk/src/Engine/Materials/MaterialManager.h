#include "Base/XML/tinyxml2/tinyxml2.h"
#include <list>
#include "Utils\TemplatedMap.h"
#include "Base\Utils\Name.h"

namespace engine{

	namespace materials{
		class MaterialManager{ //: public TemplatedMap<Material> {
		public:
			MaterialManager(const std::string path);
			~MaterialManager();
			void LoadXML();
			void ReloadXML();

		private:
			std::list<Material> materialList;
			std::string m_path;

		};		

		class Material : public CName{
		public:
			Material();
			~Material();
		};

	}

}