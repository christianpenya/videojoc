#include "RenderCmd.h"
#include "XML\XML.h"


CRenderCmd::CRenderCmd()
{

}

CRenderCmd::~CRenderCmd()
{

}


bool CRenderCmd::Load(const CXMLElement* aElement)
{
    CName::SetName(aElement->GetAttribute<std::string>("name", ""));
    m_Active = aElement->GetAttribute<bool>("enable", "true");

    return true;
}
