#pragma once
#ifndef _ENGINE_EFFECT_20170104
#define _ENGINE_EFFECT_20170104

#include "Base\Utils\Name.h"
#include "Shader.h"
#include <vector>

class CEffect : public CName
{
public:
    CEffect();
    CEffect(CXMLElement* aElement);
    virtual ~CEffect();
    void SetShader(CShader::EShaderStage aType, CShader* aShader);
    template < typename TShaderType > TShaderType* GetShader(CShader::EShaderStage aType) const
    {
        return static_cast<TShaderType*>(mShaders[aType]);
    }
    void Bind(ID3D11DeviceContext* aContext);

private:
    DISALLOW_COPY_AND_ASSIGN(CEffect);
    std::vector< CShader* > mShaders;
};

#endif //_ENGINE_EFFECT_20170104
