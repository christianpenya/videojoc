#pragma once
#ifndef _RENDERIMGUI_H_
#define _RENDERIMGUI_H_

#include "RenderCmd.h"
#include <vector>
#include "Engine\Engine.h"

class CRenderImGUI : public CRenderCmd
{
public:
    CRenderImGUI();
    virtual ~CRenderImGUI();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CRenderImGUI);
    int lTechnique = 0;
};

#endif