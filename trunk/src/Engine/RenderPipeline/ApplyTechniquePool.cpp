#include "ApplyTechniquePool.h"
#include "XML\XML.h"


CApplyTechniquePool::CApplyTechniquePool()
{

}

CApplyTechniquePool::~CApplyTechniquePool()
{

}

//Leerá el nodo:
//<apply_technique_pool name = "forward" />

bool CApplyTechniquePool::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderCmd::Load(aElement);
    if (lOk)
        m_PoolName = aElement->GetAttribute<std::string>("pool_name","forward");
    return lOk;
}

void CApplyTechniquePool::Execute(CRenderManager& lRM)
{
    //lRM.GetDeviceContext()->OMSetBlendState(m_PoolName, NULL, 0xffffffff);
}