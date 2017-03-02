#pragma once
#ifndef _ENGINE_PIXELSHADER_20170104
#define _ENGINE_PIXELSHADER_20170104

#include "Shader.h"

class CPixelShader : public CShader
{
public:
    CPixelShader(const std::string& aShaderCode);
    CPixelShader(const CXMLElement* aElement, const std::string& aPath);
    virtual ~CPixelShader();
    bool Load() override;
    void Bind(ID3D11DeviceContext* aContext) override;
    GET_SET_PTR(ID3D11PixelShader, PixelShader);

private:
    DISALLOW_COPY_AND_ASSIGN(CPixelShader);
    ID3D11PixelShader *m_pPixelShader;
    std::string GetShaderModel() override;
};

#endif //_ENGINE_PIXELSHADER_20170104
