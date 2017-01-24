#pragma once
#ifndef _APPLYTECHNIQUEPOOL_H_
#define _APPLYTECHNIQUEPOOL_H_

#include "RenderCmd.h"
#include "Render\RenderManager.h"
#include "Utils\Name.h"


class CApplyTechniquePool: public CRenderCmd
{
public:
    CApplyTechniquePool();
    virtual ~CApplyTechniquePool();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CApplyTechniquePool);
    std::string m_PoolName;
};

#endif