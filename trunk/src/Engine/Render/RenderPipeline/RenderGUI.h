#pragma once
#ifndef _RENDERGUI_H_
#define _RENDERGUI_H_

#include "RenderCmd.h"
#include "Engine\Engine.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CRenderGUI : public CRenderCmd
{
public:
    CRenderGUI();
    virtual ~CRenderGUI();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
private:

    DISALLOW_COPY_AND_ASSIGN(CRenderGUI);
    int lTechnique = 0;


};

#endif
