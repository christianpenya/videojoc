#pragma once
#ifndef _APPLY_FILTER_
#define _APPLY_FILTER_

#include "DrawQuad.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif


class CApplyFilter : public CDrawQuad
{
public:
    CApplyFilter();
    virtual ~CApplyFilter();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager &lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CApplyFilter);
    CQuad* mQuad;
};

#endif