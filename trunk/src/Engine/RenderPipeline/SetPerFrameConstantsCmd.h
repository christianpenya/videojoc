#pragma once
#ifndef _SPFCONSTANTSCMD_H_
#define _SPFCONSTANTSCMD_H_

#include "RenderCmd.h"
#include "Render\RenderManager.h"
#include "Utils\Name.h"

class CSetPerFrameConstantsCmd: public CRenderCmd
{
public:
    CSetPerFrameConstantsCmd ();
    virtual ~CSetPerFrameConstantsCmd();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CSetPerFrameConstantsCmd);
};

#endif