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

    //BUTTON IDS
    const int RELOAD_SHADER_BUTTON_ID = 0;

    //BUTTON COLOR PALETTE
    const ImColor GREEN = ImColor::HSV(0.35f, 0.8f, 0.6f);
    const ImColor GREEN_HOVER = ImColor::HSV(0.35f, 0.8f, 0.6f);
};

#endif