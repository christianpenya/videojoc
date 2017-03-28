#pragma once
#ifndef CFB_TEXTURE_H
#define CFB_TEXTURE_H

#include "Graphics\Textures\Texture.h"
#include "Utils\Defines.h"
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
    CCaptureFrameBufferTexture();
    CCaptureFrameBufferTexture(const CXMLElement* TreeNode);
    virtual ~CCaptureFrameBufferTexture();
    bool Capture(unsigned int StageId);
    ID3D11Texture2D * CapturedTexture()
    {
        return m_DataTexture;
    }
};


#endif //TEXTURE_H