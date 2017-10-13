#include "UnSetRenderTarget.h"


CUnSetRenderTarget::CUnSetRenderTarget()
{

}

CUnSetRenderTarget::~CUnSetRenderTarget()
{

}

bool CUnSetRenderTarget::Load(const CXMLElement* aElement)
{
    return true;
}


void CUnSetRenderTarget::Execute(CRenderManager& lRM)
{
    lRM.UnsetRenderTargets();
}

void CUnSetRenderTarget::DrawImgui() {}