#pragma once
#ifndef _CAPTUREFRAMEBUFFER_H_
#define _CAPTUREFRAMEBUFFER_H_

#include "Render\RenderManager.h"
#include "RenderStagedTexture.h"
#include "CaptureFrameBufferTexture.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CCaptureFrameBuffer : public CRenderStagedTexture
{
public:
    CCaptureFrameBuffer();
    virtual ~CCaptureFrameBuffer();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
private:
    DISALLOW_COPY_AND_ASSIGN(CCaptureFrameBuffer);
    CCaptureFrameBufferTexture* m_CapturedFrameBufferTexture;
};

#endif
