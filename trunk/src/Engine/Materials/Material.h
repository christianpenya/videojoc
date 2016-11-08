#include<string>
#include "Base\Utils\Name.h"

namespace engine {
	
	namespace materials {

		class CMaterial : public CName {

		public:
			CMaterial();
			~CMaterial();

		private:
			std::string path;
			std::string nom;
		};
	
	}
}