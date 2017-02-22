#pragma once
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Utils\Defines.h"
#include "Utils\Name.h"
#include "Math\Vector2.h"

#include <d3d11.h>

class CTexture:public CName
{
    enum EType
    {
        e2D_Texture	= 0,
        e2D_TextureArray,
        e3D_Texture
    };
public:
    CTexture(const std::string&  aName) : CName(aName), m_pTexture(nullptr), m_SamplerState(nullptr)
    {
    }

    virtual ~CTexture() {}
    bool Load();
    bool Reload();
    void Bind(uint32 aStageId, ID3D11DeviceContext*  aContext);
    GET_SET_PTR(ID3D11ShaderResourceView, Texture);
protected:
    ID3D11ShaderResourceView	*m_pTexture;
    ID3D11SamplerState			*m_SamplerState;
    Vect2u						mSize;
private:
    DISALLOW_COPY_AND_ASSIGN(CTexture);
    void Destroy();
};

#endif