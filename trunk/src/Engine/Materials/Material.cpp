#include "Material.h"
#include "TemplatedMaterialParameter.h"
#include "XML\XML.h"
#include <assert.h>
#include "Utils\EnumToString.h"
#include "Base\Math\Vector2.h"
#include "Base\Math\Vector3.h"
#include "Base\Math\Vector4.h"
#include "Base\Math\Color.h"
#include "Base\Utils\CheckedDelete.h"

CMaterial::~CMaterial() {
    base::utils::CheckedDelete(mParameters);
    base::utils::CheckedDelete(mTextures);
}

CMaterial::CMaterial(CXMLElement* aElement) : CName( aElement ) 
{
    //mTextures.resize(CMaterial::ETextureIndex, nullptr);
    for (tinyxml2::XMLElement *iTextureOrParameter = aElement->FirstChildElement(); iTextureOrParameter != nullptr; iTextureOrParameter = iTextureOrParameter->NextSiblingElement()) 
    {
        if (strcmp(iTextureOrParameter->Name(), "texture") == 0) 
        {
            //leemos textura
            std::string blabla = "type";
            const char *textureName = iTextureOrParameter->Attribute("type");
            const char *textureFilename = iTextureOrParameter->Attribute("filename");
            //CTexture * lTexture = new CTexture();
            //mTextures.push_back(lTexture); 
        }
        else if (strcmp(iTextureOrParameter->Name(), "parameter") == 0)
        {
            TParameterType lType;
            
            if (EnumString<TParameterType>::ToEnum(lType, iTextureOrParameter->GetAttribute<std::string>("type", "")))
            {
                CMaterialParameter* lParameter = nullptr;

#define CASE_CREATE_MATERIAL_PARAMETER( parameter_type, parameter_class_type, parameter_def_value ) \
case parameter_type: \
{ \
    lParameter = new CTemplatedMaterialParameter<parameter_class_type>(\
        iTextureOrParameter->GetAttribute<std::string>("name", ""), \
        iTextureOrParameter->GetAttribute<parameter_class_type>("value", parameter_def_value), \
        parameter_type);\
}\
break;\

            switch (lType)
            {
                CASE_CREATE_MATERIAL_PARAMETER(CMaterial::eFloat, float, 0.0f);
                CASE_CREATE_MATERIAL_PARAMETER(CMaterial::eFloat2, Vect2f, Vect2f(0.0f, 0.0f));
                CASE_CREATE_MATERIAL_PARAMETER(CMaterial::eFloat3, Vect3f, Vect3f(0.0f, 0.0f, 0.0f));
                CASE_CREATE_MATERIAL_PARAMETER(CMaterial::eFloat4, Vect4f, Vect4f(0.0f, 0.0f, 0.0f, 0.0f));
                CASE_CREATE_MATERIAL_PARAMETER(CMaterial::eColor, CColor, CColor());
            }

#undef CASE_CREATE_MATERIAL_PARAMETER
            
            if (lParameter)                 
                mParameters.push_back(lParameter);
            }
        }
    }
}