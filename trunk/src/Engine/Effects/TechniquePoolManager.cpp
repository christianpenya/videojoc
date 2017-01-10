#include "TechniquePoolManager.h"
#include "Base/XML/tinyxml2/tinyxml2.h"
#include "Base/XML/XML.h"
#include "Utils\EnumToString.h"
#include "Engine\Engine.h"
#include "Engine\Effects\EffectManager.h"

CTechniquePoolManager::CTechniquePoolManager() {}
CTechniquePoolManager::~CTechniquePoolManager() {}

bool CTechniquePoolManager::Load(const std::string& aFilename)
{
    m_Filename = aFilename;
    return Reload();
}

bool CTechniquePoolManager::Reload()
{
    bool lOk = false;

    CXMLDocument document;
    EXMLParseError error = document.LoadFile((m_Filename).c_str());

    CEffectManager& lEM = CEngine::GetInstance().GetEffectManager();

    if (base::xml::SucceedLoad(error))
    {
        CXMLElement * lTechniquePools = document.FirstChildElement("technique_pools");

        if (lTechniquePools)
        {
            for (tinyxml2::XMLElement *iPool = lTechniquePools->FirstChildElement(); iPool != nullptr; iPool = iPool->NextSiblingElement())
            {
                if (strcmp(iPool->Name(), "default_pool") == 0)
                {
                    for (tinyxml2::XMLElement *iTechnique = iPool->FirstChildElement(); iTechnique != nullptr; iTechnique = iTechnique->NextSiblingElement())
                    {
                        if (strcmp(iTechnique->Name(), "technique") == 0)
                        {
                            std::string lVertexType = iTechnique->GetAttribute<std::string>("vertex_type", "");
                            // *ALEX: esto está bien? No me cuadra buscar effecto por nombre de vertice. que efectos cargamos por defecto?
                            CTechnique*	lTechnique = new CTechnique();
                            lTechnique->SetEffect(lEM(lVertexType));
                            Add(lVertexType, lTechnique);
                        }
                    }
                }
                else if (strcmp(iPool->Name(), "pool") == 0)
                {
                    CTechniquePool* lTechniquePool = new CTechniquePool(iPool);
                    mPools.Add(lTechniquePool->GetName(), lTechniquePool);
                }
            }
            lOk = true;
        }
    }
    return lOk;
}

bool Apply(const std::string& aPoolName)
{
    // *ALEX: que significa aplicar la pool?
    return true;
}