#include "Effect.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "Engine\Engine.h"
#include "Utils\EnumToString.h"
#include "Effects\ShaderManager.h"

CEffect::CEffect() {}

CEffect::~CEffect() {}

CEffect::CEffect(CXMLElement* aElement) :
    CName(aElement->GetAttribute<std::string>("name", ""))
{
    mShaders.resize(CShader::EShaderStage::EStageCount);
    CShaderManager& l_ShaderManager = CEngine::GetInstance().GetShaderManager();

    for (tinyxml2::XMLElement *iShader = aElement->FirstChildElement(); iShader != nullptr; iShader = iShader->NextSiblingElement())
    {
        if (strcmp(iShader->Name(), "shader") == 0)
        {
            std::string lStage = iShader->GetAttribute<std::string>("stage", "");
            std::string lName = iShader->GetAttribute<std::string>("name", "");

            CShader::EShaderStage lEnumStage;
            EnumString<CShader::EShaderStage>::ToEnum(lEnumStage, lStage);
            mShaders[lEnumStage] = (l_ShaderManager.GetShader(lEnumStage, lName));
        }
    }
}

void CEffect::Bind(ID3D11DeviceContext* aContext)
{
    for (std::vector<CShader*>::iterator iShader = mShaders.begin(); iShader != mShaders.end(); ++iShader)
    {
        if ((*iShader) != nullptr)
        {
            (*iShader)->Bind(aContext);
        }
    }
}

void CEffect::SetShader(CShader::EShaderStage aType, CShader* aShader)
{
    mShaders[aType] = aShader;
}
void CEffect::Refresh()
{
    // #TODO
}
