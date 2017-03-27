#pragma once
#ifndef _DISABLE_ALPHA_BLEND_
#define _DISABLE_ALPHA_BLEND_

#include "RenderCmd.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CDisableAlphaBlend : public CRenderCmd
{
public:
    CDisableAlphaBlend();
    virtual ~CDisableAlphaBlend();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager &lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CDisableAlphaBlend);
};

#endif
