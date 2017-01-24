#pragma once
#ifndef _RENDERSCENELAYER_H_
#define _RENDERSCENELAYER_H_

#include "RenderCmd.h"
#include "Render\RenderManager.h"
#include "Utils\Name.h"


class CRenderSceneLayer : public CRenderCmd
{
public:
    CRenderSceneLayer();
    virtual ~CRenderSceneLayer();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CRenderSceneLayer);
    std::string m_LayerName;
};

#endif