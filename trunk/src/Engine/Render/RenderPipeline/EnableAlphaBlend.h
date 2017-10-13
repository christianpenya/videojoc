#pragma once
#ifndef _ENABLE_ALPHA_BLEND_
#define _ENABLE_ALPHA_BLEND_

#include "RenderCmd.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CEnableAlphaBlend : public CRenderCmd
{
public:
    CEnableAlphaBlend();
    virtual ~CEnableAlphaBlend();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager &lRM);
    virtual void DrawImgui();
private:
    ID3D11BlendState *m_AlphaBlendState;
    DISALLOW_COPY_AND_ASSIGN(CEnableAlphaBlend);
};

#endif
