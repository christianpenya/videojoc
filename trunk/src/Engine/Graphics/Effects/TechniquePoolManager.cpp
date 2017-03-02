#include "TechniquePoolManager.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "XML\XML.h"
#include "Engine\Engine.h"
#include "Graphics/Effects/EffectManager.h"
#include "TechniquePool.h"

CTechniquePoolManager::CTechniquePoolManager() {}
CTechniquePoolManager::~CTechniquePoolManager()
{
    CTemplatedMapVector<CTechnique>::Destroy();
    mPools.Destroy();
}

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
                            CTechnique*	lTechnique = new CTechnique();
                            lTechnique->SetEffect(nullptr);
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

bool  CTechniquePoolManager::Apply(const std::string& aPoolName)
{
    bool lOk = false;
    CTechniquePool* lPool = mPools(aPoolName);
    if (lPool)
    {
        for (auto iTechnique = m_ResourcesMap.begin(); iTechnique != m_ResourcesMap.end(); ++iTechnique)
        {
            (*iTechnique).second.m_Value->SetEffect((*lPool)((*iTechnique).first));
        }
        lOk = true;
    }

    return lOk;
}

std::vector<std::string> CTechniquePoolManager::GetPoolNames()
{
    std::vector<std::string> lOut;
    lOut.reserve(mPools.GetCount());

    for (unsigned int i = 0; i < mPools.GetCount(); ++i)
    {
        lOut.push_back(mPools[i]->GetName());
    }

    return lOut;
}
