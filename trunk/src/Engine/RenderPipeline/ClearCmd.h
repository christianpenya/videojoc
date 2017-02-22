#pragma once
#ifndef _CLEARCMD_H_
#define _CLEARCMD_H_

#include "RenderCmd.h"
class CRenderManager;

class CClearCmd : public CRenderCmd
{
public:
    CClearCmd();
    virtual ~CClearCmd();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CClearCmd);
    bool m_RenderTarget;
    bool m_DepthStencil;
    CColor m_Color;
};

#endif
