#pragma once
#ifndef _ENGINE_GEOMETRYSHADER_20170427
#define _ENGINE_GEOMETRYSHADER_20170427

#include "Shader.h"

class CGeometryShader : public CShader
{
public:
    CGeometryShader(const std::string& aShaderCode);
    CGeometryShader(const CXMLElement* aElement, const std::string& aPath);
    virtual ~CGeometryShader();
    bool Load() override;
    void Bind(ID3D11DeviceContext* aContext) override;
    GET_SET_PTR(ID3D11GeometryShader, GeometryShader);

private:
    DISALLOW_COPY_AND_ASSIGN(CGeometryShader);
    ID3D11GeometryShader *m_pGeometryShader;
    std::string GetShaderModel() override;
};

#endif //_ENGINE_PIXELSHADER_20170104
