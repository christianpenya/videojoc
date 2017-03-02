#pragma once
#ifndef _DRAWQUAD_H_
#define _DRAWQUAD_H_

#include "RenderStagedTexture.h"
#include "Quad.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CQuad;
class CRenderManager;

class CDrawQuad : public CRenderStagedTexture
{
public:
    CDrawQuad();
    virtual ~CDrawQuad();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CDrawQuad);
    CQuad* mQuad;
    CPixelShader* mPixelShader;
};

#endif
