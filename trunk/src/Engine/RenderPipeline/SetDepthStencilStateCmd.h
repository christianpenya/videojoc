#pragma once
#ifndef _SDSTENCILSTATECMD_H_
#define _SDSTENCILSTATECMD_H_

#include "RenderCmd.h"
#include "Render\RenderManager.h"
#include "Utils\Name.h"

class CSetDepthStencilStateCmd: public CRenderCmd
{
public:
    CSetDepthStencilStateCmd();
    virtual ~CSetDepthStencilStateCmd();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
    virtual void DrawIMGUI();
private:
    DISALLOW_COPY_AND_ASSIGN(CSetDepthStencilStateCmd);
    ID3D11DepthStencilState *m_DepthStencilState;
    bool m_EnableZTest;
    bool m_WriteZBuffer;
    bool m_EnableStencil;
    int m_ComparisonFunc;
    bool CreateDepthStencilState();
};

#endif