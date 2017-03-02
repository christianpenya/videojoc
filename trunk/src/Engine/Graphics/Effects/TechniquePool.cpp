#include "TechniquePool.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "Engine\Engine.h"
#include "Graphics/Effects/EffectManager.h"
#include "Utils/Logger.h"
#include "Utils/CheckedRelease.h"

CTechniquePool::~CTechniquePool()
{
    /* for (std::vector<CEffect*>::iterator iEffect = m_ResourcesVector.begin(); iEffect != m_ResourcesVector.end(); ++iEffect)
     {
         base::utils::CheckedRelease(*iEffect);
     }*/
    CTemplatedMapVector<CEffect>::Clear();
}

CTechniquePool::CTechniquePool(CXMLElement* aElement) :
    CName(aElement->GetAttribute<std::string>("name", ""))
{
    CEffectManager& lEM = CEngine::GetInstance().GetEffectManager();

    std::string lVertexType;
    std::string lEffect;

    for (tinyxml2::XMLElement *iTechnique = aElement->FirstChildElement(); iTechnique != nullptr; iTechnique = iTechnique->NextSiblingElement())
    {
        if (strcmp(iTechnique->Name(), "technique") == 0)
        {
            lVertexType = iTechnique->GetAttribute<std::string>("vertex_type", "");
            lEffect = iTechnique->GetAttribute<std::string>("effect", "");

            CTemplatedMapVector<CEffect>::Add(lVertexType,lEM(lEffect));
        }
    }
}
