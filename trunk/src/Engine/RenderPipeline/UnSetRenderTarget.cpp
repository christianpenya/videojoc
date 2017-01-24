#include "UnSetRenderTarget.h"


CUnSetRenderTarget::CUnSetRenderTarget()
{

}

CUnSetRenderTarget::~CUnSetRenderTarget()
{

}

bool CUnSetRenderTarget::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderStagedTexture::Load(aElement);
    return lOk;
}


void CUnSetRenderTarget::Execute(CRenderManager& lRM)
{
    lRM.UnsetRenderTargets();
}