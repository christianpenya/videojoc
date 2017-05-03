#include "ShaderManager.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "XML\XML.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"

CShaderManager::CShaderManager()
{
    m_Library.resize(CShader::EShaderStage::EStageCount);
}

CShaderManager::~CShaderManager()
{
    for (std::vector<TShadersLibrary>::iterator iShaderVectorGroupedByVertexType = m_Library.begin(); iShaderVectorGroupedByVertexType != m_Library.end(); ++iShaderVectorGroupedByVertexType)
    {
        iShaderVectorGroupedByVertexType->Destroy();
    }
}

bool CShaderManager::Load(const std::string& aFilename)
{
    bool out = false;
    m_Filename = aFilename;

    CXMLDocument document;
    EXMLParseError error = document.LoadFile((m_Filename).c_str());

    if (base::xml::SucceedLoad(error))
    {
        CXMLElement * lShaders = document.FirstChildElement("shaders");
        std::string l_Path = lShaders->GetAttribute<std::string>("path", "");

        if (lShaders)
        {
            for (tinyxml2::XMLElement *iShaderType = lShaders->FirstChildElement(); iShaderType != nullptr; iShaderType = iShaderType->NextSiblingElement())
            {
                if (strcmp(iShaderType->Name(), "vertex_shaders") == 0)
                {
                    m_Library[CShader::EShaderStage::eVertexShader].Clear();
                    for (tinyxml2::XMLElement *iVertexShader = iShaderType->FirstChildElement(); iVertexShader != nullptr; iVertexShader = iVertexShader->NextSiblingElement())
                    {
                        if (strcmp(iVertexShader->Name(), "shader") == 0)
                        {
                            CVertexShader *lVertexShader = new CVertexShader(iVertexShader, l_Path);
                            lVertexShader->Load();

                            m_Library[CShader::EShaderStage::eVertexShader].Add(lVertexShader->GetName(), lVertexShader);
                        }
                    }
                }
                else if (strcmp(iShaderType->Name(), "pixel_shaders") == 0)
                {
                    for (tinyxml2::XMLElement *iPixelShader = iShaderType->FirstChildElement(); iPixelShader != nullptr; iPixelShader = iPixelShader->NextSiblingElement())
                    {
                        if (strcmp(iPixelShader->Name(), "shader") == 0)
                        {
                            CPixelShader *lPixelShader = new CPixelShader(iPixelShader, l_Path);
                            lPixelShader->Load();
                            m_Library[CShader::EShaderStage::ePixelShader].Add(lPixelShader->GetName(), lPixelShader);
                        }
                    }
                }
                else if (strcmp(iShaderType->Name(), "geometry_shaders") == 0)
                {
                    for (tinyxml2::XMLElement *iPixelShader = iShaderType->FirstChildElement(); iPixelShader != nullptr; iPixelShader = iPixelShader->NextSiblingElement())
                    {
                        if (strcmp(iPixelShader->Name(), "shader") == 0)
                        {
                            CGeometryShader *lGeometryShader = new CGeometryShader(iPixelShader, l_Path);
                            lGeometryShader->Load();
                            m_Library[CShader::EShaderStage::eGeometryShader].Add(lGeometryShader->GetName(), lGeometryShader);
                        }
                    }
                }
            }
            out = true;
        }
    }

    return out;
}

bool CShaderManager::Reload()
{
    m_Library.clear();
    m_Library.resize(CShader::EShaderStage::EStageCount);
    return Load(m_Filename);
}

CShader* CShaderManager::GetShader(CShader::EShaderStage aStage, const std::string& aShaderName)
{
    return m_Library[aStage](aShaderName);
}

std::vector<CShader*> CShaderManager::GetShaders(CShader::EShaderStage aStage)
{
    //TODO o eliminar
    return {};
}
