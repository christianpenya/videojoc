#pragma once
#ifndef DYNAMIC_TEXTURE_H
#define DYNAMIC_TEXTURE_H

#include "Graphics\Textures\Texture.h"
#include "Utils\EnumToString.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

XML_FORWARD_DECLARATION

#include "Utils\Defines.h"

class CDynamicTexture : public CTexture
{
public:
    // https://msdn.microsoft.com/en-us/library/windows/desktop/bb173059(v=vs.85).aspx
    enum TFormatType
    {
        RGBA32_FLOAT = 2,
        RGBA8_UNORM = 28,
        R32_FLOAT = 41
    };

    CDynamicTexture(const CXMLElement *TreeNode);
    CDynamicTexture(std::string aName, Vect2u aSize);
    virtual ~CDynamicTexture();

    GET_SET_PTR(ID3D11RenderTargetView, RenderTargetView);
    GET_SET_PTR(ID3D11DepthStencilView, DepthStencilView);

protected:
    ID3D11Texture2D* m_pRenderTargetTexture;
    ID3D11RenderTargetView* m_pRenderTargetView;
    ID3D11Texture2D* m_pDepthStencilBuffer;
    ID3D11DepthStencilView* m_pDepthStencilView;
    bool                    m_CreateDepthStencilBuffer;
    TFormatType             m_FormatType;

    void Init();
    virtual bool CreateSamplerState();
};

//---------------------------------------------------------------------------------------------------------
Begin_Enum_String(CDynamicTexture::TFormatType)
{
    Enum_String_Id(CDynamicTexture::R32_FLOAT, "R32_FLOAT");
    Enum_String_Id(CDynamicTexture::RGBA32_FLOAT, "RGBA32_FLOAT");
    Enum_String_Id(CDynamicTexture::RGBA8_UNORM, "RGBA8_UNORM");
}
End_Enum_String;

#endif //TEXTURE_H