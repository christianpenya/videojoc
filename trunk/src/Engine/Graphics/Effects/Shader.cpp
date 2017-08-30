#include "Shader.h"
#include "ShaderInclude.h"
#include <fstream>
#include "XML\tinyxml2\tinyxml2.h"
#include <vector>
#include "Utils/StringUtils.h"
#include "Utils/CheckedRelease.h"
#include "Utils/FileUtils.cpp"





#include <filesystem>

CShader::CShader(const std::string& aShaderCode, EShaderStage aType) :
    m_Type(aType),
    m_ShaderCode(aShaderCode),
    m_ShaderMacros(nullptr),
    m_pBlob(nullptr)
{}

CShader::CShader(const CXMLElement* aElement, const std::string aPath, EShaderStage aType) :
    m_Type(aType),
    m_ShaderMacros(nullptr),
    m_pBlob(nullptr),
    CName(aElement->GetAttribute<std::string>("name", "")),
    m_Filename(aPath + aElement->GetAttribute<std::string>("file", "")),
    m_EntryPoint(aElement->GetAttribute<std::string>("entry_point", "")),
    m_Preprocessor(aElement->GetAttribute<std::string>("preprocessor", ""))
{
}

CShader::~CShader()
{
    base::utils::CheckedRelease(m_pBlob);
    base::utils::CheckedDelete(m_ShaderMacros);
}

bool CShader::Load()
{
    /* if (!m_Filename.empty())
     {
         std::ifstream lStream(m_Filename);
         m_ShaderCode = std::string((std::istreambuf_iterator<char>(lStream)),
                                    (std::istreambuf_iterator<char>()));
     }

     if (!m_ShaderCode.empty())
     {
         std::string lCompiledFileName = m_Filename + "." + CreateMacroHash() + ".blob";
         std::wstring tmp = base::utils::String2WString(lCompiledFileName);
         LPCWSTR lCompiledFileNameLPCWSTR = tmp.c_str();

         std::wstring lWideFilename = base::utils::String2WString(m_Filename);
         LPCWSTR lFilename = lWideFilename.c_str();

         CreateShaderMacro();
         HRESULT hr;

         if (base::utils::DoesFileExist(lCompiledFileName))
         {
             if (base::utils::IsFileOlder(lCompiledFileName, m_Filename))
             {
                 //hr = D3DCompileFromFile(lFilename,  )
                 m_pBlob = ShaderUtils::CompileShader(m_ShaderCode, m_EntryPoint, GetShaderModel(), m_ShaderMacros);
                 hr = D3DWriteBlobToFile(m_pBlob, lCompiledFileNameLPCWSTR, true);
             }
             else
             {
                 //std::ifstream input(lCompiledFileName, std::ios::binary);
                 //m_pBlob = input.get();
                 hr = D3DReadFileToBlob(lCompiledFileNameLPCWSTR, &m_pBlob);
                 assert(m_pBlob != nullptr);
             }
         }
         else
         {
             m_pBlob = ShaderUtils::CompileShader(m_ShaderCode, m_EntryPoint, GetShaderModel(), m_ShaderMacros);
             hr = D3DWriteBlobToFile(m_pBlob, lCompiledFileNameLPCWSTR, true);
         }

         assert(SUCCEEDED(hr));
     }


     return m_pBlob != nullptr;*/
    if (!m_Filename.empty())
    {
        std::ifstream lStream(m_Filename);
        m_ShaderCode = std::string((std::istreambuf_iterator<char>(lStream)),
                                   (std::istreambuf_iterator<char>()));
    }

    if (!m_ShaderCode.empty())
    {
        CreateShaderMacro();
        m_pBlob = ShaderUtils::CompileShader(m_ShaderCode, m_EntryPoint, GetShaderModel(), m_ShaderMacros);
    }

    return m_pBlob != nullptr;
}

bool CShader::Reload()
{
    // TODO
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

std::string CShader::CreateMacroHash() const
{
    std::hash<std::string> hash;
    return std::to_string(hash(m_Preprocessor + m_EntryPoint));
}
