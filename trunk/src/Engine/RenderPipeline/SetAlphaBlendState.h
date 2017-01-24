#pragma once
#ifndef _SETALPHABLENDSTATE_H_
#define _SETALPHABLENDSTATE_H_

#include "RenderCmd.h"
#include "Render\RenderManager.h"
#include "Utils\Name.h"


class CSetAlphaBlendState : public CRenderCmd
{
public:
    CSetAlphaBlendState();
    virtual ~CSetAlphaBlendState();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CSetAlphaBlendState);
    ID3D11BlendState *m_AlphaBlendState;
};

#endif