#include "Shader.h"
#include "ShaderInclude.h"
#include <fstream>
#include "XML\tinyxml2\tinyxml2.h"
#include <vector>
#include "Utils/StringUtils.h"

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
    //m_pBlob->Release();
}

bool CShader::Load()
{
    if (!m_Filename.empty())
    {
        std::ifstream lStream(m_Filename);
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

bool CShader::Reload()
{
    // #TODO
    return false;
}

void CShader::CreateShaderMacro()
{
    m_PreprocessorMacros.clear();

    if (m_Preprocessor.empty())
    {
        m_ShaderMacros = NULL;
        return;
    }

    std::vector<std::string> l_PreprocessorItems = base::utils::Split(m_Preprocessor, ';');
    m_ShaderMacros = new D3D10_SHADER_MACRO[l_PreprocessorItems.size() + 1];

    for (size_t i = 0; i < l_PreprocessorItems.size(); ++i)
    {
        std::vector<std::string> l_PreprocessorItem = base::utils::Split(
                    l_PreprocessorItems[i], '=');
        if (l_PreprocessorItem.size() == 1)
        {
            m_PreprocessorMacros.push_back(l_PreprocessorItems[i]);
            m_PreprocessorMacros.push_back("1");
        }
        else if (l_PreprocessorItem.size() == 2)
        {
            m_PreprocessorMacros.push_back(l_PreprocessorItem[0]);
            m_PreprocessorMacros.push_back(l_PreprocessorItem[1]);
        }
        else
        {
            base::utils::DeleteArray(m_ShaderMacros);
            return;
        }
    }
    for (size_t i = 0; i < l_PreprocessorItems.size(); ++i)
    {
        m_ShaderMacros[i].Name = m_PreprocessorMacros[i * 2].c_str();
        m_ShaderMacros[i].Definition = m_PreprocessorMacros[(i * 2) + 1].c_str();
    }
    m_ShaderMacros[l_PreprocessorItems.size()].Name = NULL;
    m_ShaderMacros[l_PreprocessorItems.size()].Definition = NULL;
}

