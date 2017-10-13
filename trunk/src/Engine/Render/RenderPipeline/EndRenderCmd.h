#pragma once
#ifndef _ENDRENDERCMD_H_
#define _ENDRENDERCMD_H_

#include "RenderCmd.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CRenderManager;

class CEndRenderCmd : public CRenderCmd
{
public:
    CEndRenderCmd();
    virtual ~CEndRenderCmd();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
    virtual void DrawImgui();
private:
    DISALLOW_COPY_AND_ASSIGN(CEndRenderCmd);
};

#endif