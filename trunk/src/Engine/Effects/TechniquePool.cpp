#include "TechniquePool.h"
#include <string>
#include "XML\tinyxml2\tinyxml2.h"
#include "Engine\Engine.h"
#include "Mesh\VertexTypes.h"
#include "Effects\EffectManager.h"

CTechniquePool::~CTechniquePool() {}

CTechniquePool::CTechniquePool(CXMLElement* aElement) :
    CName(aElement->GetAttribute<std::string>("name", ""))
{
    CEffectManager& lEM = CEngine::GetInstance().GetEffectManager();

    for (tinyxml2::XMLElement *iTechnique = aElement->FirstChildElement(); iTechnique != nullptr; iTechnique = iTechnique->NextSiblingElement())
    {
        if (strcmp(iTechnique->Name(), "technique") == 0)
        {
            std::string lVertexType = iTechnique->GetAttribute<std::string>("vertex_type", "");
            std::string lEffect = iTechnique->GetAttribute<std::string>("effect", "");

            Add(lVertexType,lEM(lEffect));
        }
    }
}