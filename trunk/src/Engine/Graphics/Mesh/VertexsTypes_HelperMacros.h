#ifndef _VERTEX_TYPES_HELPER_MACROS_HH_
#define _VERTEX_TYPES_HELPER_MACROS_HH_
#pragma once

// VARIABLES DECLARATIONS
#define DUMMY     float dummy;
#define POSITION  Vect3f position;
#define COLOR     Vect4f color;
#define NORMAL    Vect3f normal;
#define TANGENT   Vect4f tangent;
#define BINORMAL  Vect4f binormal;
#define BUMP      NORMAL TANGENT BINORMAL
#define UV        Vect2f uv;
#define UV2       Vect2f uv2;
#define POSITION4 Vect4f position;
#define WEIGHT    Vect4f weight;
#define INDICES   Vect4f indices;

// FORMAT
#define INPUT_SLOT0 0
#define SEMANTIC_INDEX0 0
#define SEMANTIC_INDEX1 1
#define PER_VERTEX D3D11_INPUT_PER_VERTEX_DATA, 0
#define R_32F    DXGI_FORMAT_R32_FLOAT
#define RG_32F   DXGI_FORMAT_R32G32_FLOAT
#define RGB_32F  DXGI_FORMAT_R32G32B32_FLOAT
#define RGBA_32F DXGI_FORMAT_R32G32B32A32_FLOAT

// LAYOUT DEFINITIONS
#define LAYOUT_DUMMY(Offset)    { "POSITION", SEMANTIC_INDEX0, R_32F,  INPUT_SLOT0, Offset, PER_VERTEX }
#define LAYOUT_POSITION(Offset) { "POSITION", SEMANTIC_INDEX0, RGB_32F,  INPUT_SLOT0, Offset, PER_VERTEX }
#define LAYOUT_COLOR(Offset)    { "COLOR",    SEMANTIC_INDEX0, RGBA_32F, INPUT_SLOT0, Offset, PER_VERTEX }
#define LAYOUT_NORMAL(Offset)   { "NORMAL",   SEMANTIC_INDEX0, RGB_32F,  INPUT_SLOT0, Offset, PER_VERTEX }
#define LAYOUT_TANGENT(Offset)  { "TANGENT",  SEMANTIC_INDEX0, RGBA_32F, INPUT_SLOT0, Offset, PER_VERTEX }
#define LAYOUT_BINORMAL(Offset) { "BINORMAL", SEMANTIC_INDEX0, RGBA_32F, INPUT_SLOT0, Offset, PER_VERTEX }
#define LAYOUT_BUMP(Offset)     LAYOUT_NORMAL(Offset), LAYOUT_TANGENT(Offset + 16), LAYOUT_BINORMAL(Offset + 16 + 16)
#define LAYOUT_UV(Offset)       { "TEXCOORD", SEMANTIC_INDEX0, RG_32F,   INPUT_SLOT0, Offset, PER_VERTEX }
#define LAYOUT_UV2(Offset)      { "TEXCOORD", SEMANTIC_INDEX1, RG_32F,   INPUT_SLOT0, Offset, PER_VERTEX }
#define LAYOUT_WEIGHT(Offset)   { "BLENDWEIGHT", SEMANTIC_INDEX0, RGBA_32F,   INPUT_SLOT0, Offset, PER_VERTEX }
#define LAYOUT_INDICES(Offset)   { "BLENDINDICES", SEMANTIC_INDEX0, RGBA_32F,   INPUT_SLOT0, Offset, PER_VERTEX }

// VERTEX_TYPE
#define GET_VERTEX_FLAGS( Flags ) \
static const uint32 GetVertexFlags() \
{ \
  return Flags; \
} \

#define BEGIN_INPUT_LAYOUT \
static bool CreateInputLayout \
(\
  CRenderManager &RenderManager, \
  ID3DBlob *VSBlob, \
  ID3D11InputLayout **VertexLayout \
)\
{ \
D3D11_INPUT_ELEMENT_DESC l_Layout[] = \

#define END_INPUT_LAYOUT \
 ; \
  UINT l_NumElements = ARRAYSIZE(l_Layout); \
  HRESULT l_HR = RenderManager.GetDevice()->CreateInputLayout(\
    l_Layout, \
    l_NumElements, \
    VSBlob->GetBufferPointer(), \
    VSBlob->GetBufferSize(), \
    VertexLayout); \
  return SUCCEEDED(l_HR); \
} \

#endif // _VERTEX_TYPES_HELPER_MACROS_HH_
