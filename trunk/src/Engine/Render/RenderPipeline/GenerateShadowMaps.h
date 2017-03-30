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
    bool Load(const CXMLElement* aElement) override;
    void Execute(CRenderManager& lRM) override;
private:
    DISALLOW_COPY_AND_ASSIGN(CGenerateShadowMaps);
};

#endif
