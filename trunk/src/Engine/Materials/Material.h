#pragma once

#ifndef _ENGINE_MATERIAL_CPB_2016112420428_H
#define _ENGINE_MATERIAL_CPB_2016112420428_H

#include<string>
#include<vector>
#include "Base\Utils\Name.h"
#include "Utils\EnumToString.h"

class CTechnique;
class CTexture;
class CMaterialParameter;

class CMaterial : public CName
{
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
	CMaterial(CXMLElement* aElement);
	virtual ~CMaterial();
	void Apply();
    void SetTechnique(CTechnique* aTechnique) { mTechnique = aTechnique; };
    CTechnique* GetTechnique() { return mTechnique; };

private:
	DISALLOW_COPY_AND_ASSIGN(CMaterial);

    CTechnique* mTechnique;
	std::vector< CTexture* > mTextures;
	std::vector< CMaterialParameter *> mParameters;
};

Begin_Enum_String(CMaterial::ETextureIndex)
{
    Enum_String_Id(CMaterial::eDiffuse, "diffuse");
    Enum_String_Id(CMaterial::eBump, "bump");
    Enum_String_Id(CMaterial::eLightMap, "lightmap");
    Enum_String_Id(CMaterial::eSpecular, "specular");
}
End_Enum_String;

//---------------------------------------------------------------------------------------------------------
Begin_Enum_String(CMaterial::TParameterType)
{
    Enum_String_Id(CMaterial::eFloat, "float");
    Enum_String_Id(CMaterial::eFloat2, "float2");
    Enum_String_Id(CMaterial::eFloat3, "float3");
    Enum_String_Id(CMaterial::eFloat4, "float4");
    Enum_String_Id(CMaterial::eColor, "color");
}
End_Enum_String;

#endif //_ENGINE_MATERIAL_CPB_2016112420428_H
