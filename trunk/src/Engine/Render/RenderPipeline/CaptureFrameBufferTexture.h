#pragma once
#ifndef CFB_TEXTURE_H
#define CFB_TEXTURE_H

#include "Graphics\Textures\Texture.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

XML_FORWARD_DECLARATION

class CCaptureFrameBufferTexture : public CTexture
{
protected:
    ID3D11Texture2D     *m_DataTexture;
    void Init();
    bool CreateSamplerState();
public:
    CCaptureFrameBufferTexture(const CXMLElement*TreeNode);
    virtual ~CCaptureFrameBufferTexture();
    bool Capture(unsigned int StageId);
};


#endif //TEXTURE_H