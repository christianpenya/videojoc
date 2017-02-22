#pragma once
#ifndef _SETRENDERTARGET_H_
#define _SETRENDERTARGET_H_

#include "RenderStagedTexture.h"

class CSetRenderTarget : public CRenderStagedTexture
{
public:
    CSetRenderTarget();
    virtual ~CSetRenderTarget();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CSetRenderTarget);
};

#endif