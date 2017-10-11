#pragma once
#ifndef _DEFERRED_SHADING_
#define _DEFERRED_SHADING_

#include "DrawQuad.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CDeferredShading : public CDrawQuad
{
public:
    CDeferredShading();
    virtual ~CDeferredShading();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager &lRM);
    virtual void DrawImgui();
private:
    bool CreateBlendState();
    DISALLOW_COPY_AND_ASSIGN(CDeferredShading);
    ID3D11BlendState *m_EnabledAlphaBlendState;
};

#endif
