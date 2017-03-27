#pragma once
#ifndef _GENERATE_SHADOWS_
#define _GENERATE_SHADOWS_

#include "RenderCmd.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CGenerateShadowMaps : public CRenderCmd
{
public:
    CGenerateShadowMaps();
    virtual ~CGenerateShadowMaps();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CGenerateShadowMaps);
};

#endif
