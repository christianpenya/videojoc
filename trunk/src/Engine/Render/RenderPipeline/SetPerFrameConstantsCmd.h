#pragma once
#ifndef _SPFCONSTANTSCMD_H_
#define _SPFCONSTANTSCMD_H_

#include "RenderCmd.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CSetPerFrameConstantsCmd: public CRenderCmd
{
public:
    CSetPerFrameConstantsCmd ();
    virtual ~CSetPerFrameConstantsCmd();
    bool Load(const CXMLElement* aElement);
    void UpdateConstants();
    virtual void Execute(CRenderManager& lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CSetPerFrameConstantsCmd);
};

#endif
