#pragma once
#ifndef _GENERATE_SHADOWS_
#define _GENERATE_SHADOWS_

#include "RenderCmd.h"
#include <vector>

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CLight;

class CGenerateShadowMaps : public CRenderCmd
{
public:
    CGenerateShadowMaps();
    virtual ~CGenerateShadowMaps();
    bool Load(const CXMLElement* aElement) override;
    void Execute(CRenderManager& lRM) override;
    bool Refresh(); //TODO cuando se refresque escena hace falta actualizar las luces que generan sombras
private:
    DISALLOW_COPY_AND_ASSIGN(CGenerateShadowMaps);
    std::vector<CLight*> m_ShadowLights;
};

#endif
