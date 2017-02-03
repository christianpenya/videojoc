#pragma once
#ifndef _ENGINE_SHADERINCLUDE_20170104
#define _ENGINE_SHADERINCLUDE_20170104

#include <string>
#include <fstream>
#include "Utils\CheckedDelete.h"
#include "Utils\CheckedRelease.h"
#include "Render\RenderManager.h"

static std::string sShadersDirectory = "data/shaders/";

namespace ShaderUtils
{
class CShaderInclude : public ID3DInclude
{
public:
    CShaderInclude() {}

    HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
    {
        std::ifstream lBinaryFile(sShadersDirectory + pFileName, std::ios::in | std::ios::binary | std::ios::ate);
        HRESULT lHR = E_FAIL;
        if (lBinaryFile.is_open())
        {
            std::streamoff fileSize = lBinaryFile.tellg();
            char* buf = new char[(uint32)fileSize];
            lBinaryFile.seekg(0, std::ios::beg);
            lBinaryFile.read(buf, fileSize);
            lBinaryFile.close();
            *ppData = buf;
            *pBytes = (UINT)fileSize;
            lHR = S_OK;
        }

        return lHR;
    }

    HRESULT __stdcall Close(LPCVOID pData)
    {
        char* buf = (char*)pData;
        base::utils::DeleteArray(buf);
        return S_OK;
    }
};

ID3DBlob* CompileShader( const std::string& aShader,
                         const std::string& aEntryPoint,
                         const std::string& aShaderModel,
                         const D3D_SHADER_MACRO* aDefines)
{
    UINT lFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
    lFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* lErrorBlob = nullptr;
    ID3DBlob* lShaderBlob = nullptr;
    CShaderInclude* lShaderInclude = new CShaderInclude();
    HRESULT lHR = D3DCompile(aShader.c_str(), aShader.size(), sShadersDirectory.c_str(), aDefines, lShaderInclude, aEntryPoint.c_str(), aShaderModel.c_str(), 0, 0, &lShaderBlob, &lErrorBlob);

    if (FAILED(lHR))
    {
        //TODO
        //LOG_ERROR_APPLICATION("Error compiling shader\n\n");
        if (lErrorBlob)
        {
            //LOG_ERROR_APPLICATION("%s\n --------- %s \n ---------", (char*)lErrorBlob->GetBufferPointer(), aShader.c_str());
        }
        char *l_Error = (char*)lErrorBlob->GetBufferPointer();
        base::utils::CheckedRelease(lErrorBlob);
        base::utils::CheckedRelease(lShaderBlob);
    }
    base::utils::CheckedDelete(lShaderInclude);
    return lShaderBlob;
}
}

#endif //_ENGINE_SHADERINCLUDE_20170104
