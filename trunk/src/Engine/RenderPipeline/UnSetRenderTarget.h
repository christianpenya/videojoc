#pragma once
#ifndef _UNSETRENDERTARGET_H_
#define _UNSETRENDERTARGET_H_

#include "Render\RenderManager.h"
#include "Utils\Name.h"
#include "RenderStagedTexture.h"


class CUnSetRenderTarget : public CRenderStagedTexture
{
public:
    CUnSetRenderTarget();
    virtual ~CUnSetRenderTarget();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CUnSetRenderTarget);
};

#endif