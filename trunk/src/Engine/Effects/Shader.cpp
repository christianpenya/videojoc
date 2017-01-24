#include "Shader.h"
#include "ShaderInclude.h"
#include <fstream>
#include "XML\tinyxml2\tinyxml2.h"

CShader::CShader(const std::string& aShaderCode, EShaderStage aType) :
    m_ShaderCode(aShaderCode),
    m_Type(aType)
{
}

CShader::CShader(const CXMLElement* aElement, EShaderStage aType) :
    m_Type(aType),
    CName(aElement->GetAttribute<std::string>("name", ""))
{
    m_Filename = aElement->GetAttribute<std::string>("file", "");
    m_EntryPoint = aElement->GetAttribute<std::string>("entry_point", "");
    m_Preprocessor = aElement->GetAttribute<std::string>("preprocessor", "");
    m_Type = aType;
}

CShader::~CShader()
{
    m_pBlob->Release();
}

bool CShader::Load()
{
    if (!m_Filename.empty())
    {
        std::ifstream lStream("data/shaders/" + m_Filename);
        m_ShaderCode = std::string((std::istreambuf_iterator<char>(lStream)),
                                   (std::istreambuf_iterator<char>()));
    }

    if (!m_ShaderCode.empty())
    {
        const D3D_SHADER_MACRO lDefines[] = { "EXAMPLE_DEFINE", "1", NULL, NULL };
        m_pBlob = ShaderUtils::CompileShader(m_ShaderCode, m_EntryPoint, GetShaderModel(), lDefines);
    }

    return m_pBlob != nullptr;
}
