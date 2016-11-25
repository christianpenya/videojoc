#pragma once

#ifndef _ENGINE_MATERIAL_CPB_2016112420428_H
#define _ENGINE_MATERIAL_CPB_2016112420428_H

#include<string>
#include "Base\Utils\Name.h"
//#include "MaterialParameter.h"
//#include "Texture.h"
		class CMaterial : public CName {

		public:     
			enum ETextureIndex { 
				eDiffuse = 0, 
				eBump, 
				eLightMap, 
				eSpecular, 
				eIndexCount 
			};

			enum TParameterType { 
				eFloat = 0, 
				eFloat2, 
				eFloat3, 
				eFloat4, 
				eColor, 
				eParametersCount 
			};

		public:     
			CMaterial(const CXMLElement* aElement);     
			virtual ~CMaterial();     
			void Apply();

		private:     
			DISALLOW_COPY_AND_ASSIGN(CMaterial);     
//			std::vector< CTexture* >           mTextures;     std::vector< CMaterialParameter *> mParameters;
		};

#endif //_ENGINE_MATERIAL_CPB_2016112420428_H
