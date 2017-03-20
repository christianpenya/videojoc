#include "UnSetRenderTarget.h"


CUnSetRenderTarget::CUnSetRenderTarget()
{

}

CUnSetRenderTarget::~CUnSetRenderTarget()
{

}

bool CUnSetRenderTarget::Load(const CXMLElement* aElement)
{
    //TODO otro load?
    bool lOk = CRenderStagedTexture::Load(aElement);
    return lOk;
}


void CUnSetRenderTarget::Execute(CRenderManager& lRM)
{
    lRM.UnsetRenderTargets();
}