#pragma once
#ifndef _BEGINRENDERCMD_H_
#define _BEGINRENDERCMD_H_

#include "RenderCmd.h"
#include "Render\RenderManager.h"
#include "Utils\Name.h"

class CBeginRenderCmd : public CRenderCmd
{
public:
    CBeginRenderCmd();
    virtual ~CBeginRenderCmd();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CBeginRenderCmd);
};

#endif