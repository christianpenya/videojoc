#pragma once
#ifndef _RENDERCMD_H_
#define _RENDERCMD_H_

#include "Utils\Name.h"
#include "Render/RenderManager.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CRenderCmd : public CName
{
public:
    CRenderCmd();
    virtual ~CRenderCmd();
    virtual bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM) = 0;
    GET_SET_BOOL(Active);
private:
    DISALLOW_COPY_AND_ASSIGN(CRenderCmd);
    bool m_Active;
};

#endif
