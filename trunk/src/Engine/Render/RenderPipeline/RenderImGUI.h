#pragma once
#ifndef _RENDERIMGUI_H_
#define _RENDERIMGUI_H_

#include "RenderCmd.h"
#include "Engine\Engine.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

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
    const int RELOAD_RENDERPIPELINE_BUTTON_ID = 1;
    const int RELOAD_LIGHTS_BUTTON_ID = 2;

    //BUTTON COLOR PALETTE
    const ImColor GREEN = ImColor::HSV(0.35f, 0.8f, 0.6f);
    const ImColor GREEN_HOVER = ImColor::HSV(0.35f, 0.8f, 0.6f);

    void Reloads();

};

#endif
