#pragma once
#ifndef _SETRENDERTARGET_H_
#define _SETRENDERTARGET_H_

#include "RenderStagedTexture.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CSetRenderTarget : public CRenderStagedTexture
{
public:
    CSetRenderTarget();
    virtual ~CSetRenderTarget();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
    virtual void DrawImgui();
private:
    DISALLOW_COPY_AND_ASSIGN(CSetRenderTarget);
};

#endif
