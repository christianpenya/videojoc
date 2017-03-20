#pragma once
#ifndef _CLEARCMD_H_
#define _CLEARCMD_H_

#include "RenderCmd.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CRenderManager;

class CClearCmd : public CRenderCmd
{
public:
    CClearCmd();
    virtual ~CClearCmd();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
    virtual void DrawImGui();
private:
    DISALLOW_COPY_AND_ASSIGN(CClearCmd);
    bool m_RenderTarget;
    bool m_DepthStencil;
    CColor m_Color;
};

#endif
