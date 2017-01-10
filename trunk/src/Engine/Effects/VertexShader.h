#pragma once
#ifndef _ENGINE_VERTEXSHADER_20170104
#define _ENGINE_VERTEXSHADER_20170104

#include "Shader.h"
#include <string>
#include "Math\MathUtils.h"

class CVertexShader : public CShader {

public:    
    CVertexShader(const std::string& aShaderCode, uint32 aVertexFlags);
    CVertexShader(const CXMLElement* aElement);
    virtual ~CVertexShader();
    virtual bool Load();
    virtual void Bind(ID3D11DeviceContext* aContext);
    
    GET_SET_REF(uint32, VertexFlags);
    GET_SET_PTR(ID3D11VertexShader, VertexShader);    
    GET_SET_PTR(ID3D11InputLayout, VertexLayout);

private:    
    DISALLOW_COPY_AND_ASSIGN(CVertexShader);    
    uint32  m_VertexFlags;    
    ID3D11VertexShader *m_pVertexShader;    
    ID3D11InputLayout *m_pVertexLayout;
    virtual std::string GetShaderModel();
};

#endif //_ENGINE_VERTEXSHADER_20170104
