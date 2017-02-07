#pragma once
#ifndef _ENGINE_SHADERMANAGER_20170104
#define _ENGINE_SHADERMANAGER_20170104

#include "Shader.h"
#include <string>
#include "Utils\TemplatedMapVector.h"

class CShaderManager
{
public:
    CShaderManager();
    virtual ~CShaderManager();
    bool Load(const std::string& aFilename);
    bool Reload();
    CShader* GetShader(CShader::EShaderStage aStage, const std::string& aShaderName);
    std::vector<CShader*> GetShaders(CShader::EShaderStage aStage);
private:
    DISALLOW_COPY_AND_ASSIGN(CShaderManager);
    typedef base::utils::CTemplatedMapVector<CShader> TShadersLibrary;
    std::vector< TShadersLibrary > m_Library;
    std::string m_Filename;
};

#endif //_ENGINE_SHADERMANAGER_20170104
