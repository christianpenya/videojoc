#include "DisableAlphaBlend.h"

CDisableAlphaBlend::CDisableAlphaBlend()
{

}

CDisableAlphaBlend::~CDisableAlphaBlend()
{

}

bool CDisableAlphaBlend::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderCmd::Load(aElement);
    return lOk;
}


void CDisableAlphaBlend::Execute(CRenderManager &lRM)
{
    lRM.GetDeviceContext()->OMSetBlendState(NULL, NULL, 0xffffffff);
}

void CDisableAlphaBlend::DrawImgui() {}