#include "Quad.h"
#include "Mesh\TemplatedGeometry.h"
#include "Mesh\VertexTypes.h"
#include "Engine\Engine.h"

#include "Materials\Material.h"
#include "Materials\MaterialManager.h"

#include "Effects\Effect.h"
#include "Effects\PixelShader.h"
#include "Effects\VertexShader.h"

static CVertexShader sQuadVertexShader(
    "struct PS_INPUT\n"
    "{\n"
    "  float4 Pos : SV_POSITION;\n"
    "  float2 UV : TEXCOORD0;\n"
    "};\n"
    "PS_INPUT quadVS(uint vI : SV_VERTEXID)"
    "{\n"
    "  PS_INPUT l_Output = (PS_INPUT)0;\n"
    "  l_Output.UV = float2(vI & 1, vI >> 1);\n"
    "  l_Output.Pos = float4((l_Output.UV.x - 0.5f) * 2, -(l_Output.UV.y - 0.5f) * 2, 0, 1);\n"
    "  return l_Output;\n"
    "}\n", VertexTypes::PositionUV::GetVertexFlags());

static bool sQuadVertexShaderLoaded = false;

/* VertexTypes::Dummy::GetVertexFlags());
Vertex::Dummy lScreenQuadVertexDummy[4] = { 0,0,0,0 };
CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();
CVertexBuffer<Vertex::Dummy> * lVB = new CVertexBuffer<Vertex::Dummy>(lRenderManager, lScreenQuadVertexDummy, 4);
mGeometry = new CGeometryTriangleStrip< Vertex::Dummy >(lVB);


static CVertexShader sQuadVertexShader(
"struct VS_INPUT\n"
"{\n"
"  float3 Pos : POSITION;\n"
"  float2 UV  : TEXCOORD0; \n"
"};\n"
"struct PS_INPUT\n"
"{\n"
"  float4 Pos : SV_POSITION;\n"
"  float2 UV : TEXCOORD0;\n"
"};\n"
"PS_INPUT quadVS(VS_INPUT IN)\n"
"{\n"
"  PS_INPUT l_Output = (PS_INPUT)0;\n"
"  l_Output.Pos = float4( IN.Pos.xyz, 1.0);\n"
"  l_Output.UV = IN.UV;\n"
"  return l_Output;\n"
"}\n",
Vertex::PositionUV::GetVertexFlags());
*/
CQuad::CQuad()
    : mGeometry(nullptr)
    , mEffect(new CEffect())
{
}

CQuad::~CQuad()
{

}

bool CQuad::Init(CPixelShader* aPS)
{
    bool lOk = false;
    CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();
    VertexTypes::PositionUV lScreenVertexsQuad[4] =
    {
        { Vect3f(-1.0f, 1.0f, 0.5f), Vect2f(0.0f, 0.0f) },
        { Vect3f(-1.0f, -1.0f, 0.5f), Vect2f(0.0f, 1.0f) },
        { Vect3f(1.0f, 1.0f, 0.5f), Vect2f(1.0f, 0.0f) },
        { Vect3f(1.0f, -1.0f, 0.5f), Vect2f(1.0f, 1.0f) }
    };

    CVertexBuffer<VertexTypes::PositionUV> * lVB = new CVertexBuffer<VertexTypes::PositionUV>(lRenderManager, lScreenVertexsQuad, 4);
    mGeometry = new CGeometryTriangleStrip< VertexTypes::PositionUV >(lVB);

    if (!sQuadVertexShaderLoaded)
    {
        sQuadVertexShaderLoaded = true;
        sQuadVertexShader.SetEntryPoint("quadVS");
        lOk = sQuadVertexShader.Load();
    }

    mEffect->SetShader(CShader::eVertexShader, &sQuadVertexShader);
    mEffect->SetShader(CShader::ePixelShader, aPS);
    return lOk;
}

bool CQuad::Render()
{
    CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();
    ID3D11DeviceContext* lContext = lRenderManager.GetDeviceContext();
    mEffect->Bind(lContext);
    return mGeometry->Render(lContext);
}

