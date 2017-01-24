#pragma once
#ifndef CFB_TEXTURE_H
#define CFB_TEXTURE_H

#include "Textures\Texture.h"

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