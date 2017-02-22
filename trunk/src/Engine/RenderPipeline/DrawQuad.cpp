#include "DrawQuad.h"
#include "Effects\PixelShader.h"
#include "Render\RenderManager.h"

CDrawQuad::CDrawQuad(): mQuad(nullptr), mPixelShader(nullptr) {}
CDrawQuad::~CDrawQuad() {}

bool CDrawQuad::Load(const CXMLElement * aElement)
{
    bool lOk = CRenderStagedTexture::Load(aElement);
    // TODO: Read the PS from the shader library with the given element into the XML
    mPixelShader = new CPixelShader(
        "Texture2D T0Texture: register( t0 );"
        "SamplerState S0Sampler : register(s0);"
        "cbuffer Material : register(b0)"
        "{"
        "float4 color;"
        "};"
        "struct PS_INPUT\n"
        "{\n"
        " float4 Pos : SV_POSITION;\n"
        " float2 UV : TEXCOORD0;\n"
        "};\n"
        "float4 quadPS(PS_INPUT IN) : SV_Target"
        "{"
        "return float4(1,0,0,1);"
        "//return T0Texture.Sample(S0Sampler, IN.UV) * color;"
        "}");
    mPixelShader->SetEntryPoint("quadPS");
    lOk &= mPixelShader->Load();
    return lOk;
}

void CDrawQuad::Execute(CRenderManager& lRM)
{
    lRM.SetViewport(m_ViewportPosition, m_ViewportSize);
    mQuad->Render();
    lRM.ResetViewport();
}
