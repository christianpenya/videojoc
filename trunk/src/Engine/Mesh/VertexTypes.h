#pragma once
#ifndef _ENGINE_VERTEXTYPES_20161229
#define _ENGINE_VERTEXTYPES_20161229

#include "VertexsTypes_HelperMacros.h"
#include "Math\Vector3.h"
#include "Render\RenderManager.h"
#include <string>

namespace VertexTypes
{
enum EFlags
{
    ePosition = 0x0001,
    eNormal = 0x0002,
    eTangent = 0x0004,
    eBinormal = 0x0008,
    eUV = 0x0010,
    eUV2 = 0x0020,
    eColor = 0x0040,
    ePosition4 = 0x0080,
    eDummy = 0x0100,
    eBump = eNormal | eTangent | eBinormal,
};

struct PositionUV
{
    POSITION;
    UV;
    GET_VERTEX_FLAGS(ePosition | eUV);
    BEGIN_INPUT_LAYOUT
    {
        LAYOUT_POSITION(0),
        LAYOUT_UV(12),
    }
    END_INPUT_LAYOUT
};

struct PositionNormal
{
    POSITION;
    NORMAL;
    GET_VERTEX_FLAGS(ePosition | eNormal);
    BEGIN_INPUT_LAYOUT
    {
        LAYOUT_POSITION(0),
        LAYOUT_NORMAL(12),
    }
    END_INPUT_LAYOUT
};

struct PositionNormalUV
{
    POSITION;
    NORMAL;
    UV;
    GET_VERTEX_FLAGS(ePosition | eNormal | eUV);
    BEGIN_INPUT_LAYOUT
    {
        LAYOUT_POSITION(0),
        LAYOUT_NORMAL(12),
        LAYOUT_UV(24),
    }
    END_INPUT_LAYOUT
};

struct PositionBump
{
    POSITION;
    BUMP;
    GET_VERTEX_FLAGS(ePosition | eBump);
    BEGIN_INPUT_LAYOUT
    {
        LAYOUT_POSITION(0),
        LAYOUT_BUMP(12),
    }
    END_INPUT_LAYOUT
};

struct PositionBumpUV
{
    POSITION;
    BUMP;
    UV;
    GET_VERTEX_FLAGS(ePosition | eBump | eUV);
    BEGIN_INPUT_LAYOUT
    {
        LAYOUT_POSITION(0),
        LAYOUT_BUMP(12),
        LAYOUT_UV(56),
    }
    END_INPUT_LAYOUT
};

struct PositionBumpUVUV2
{
    POSITION;
    BUMP;
    UV;
    UV2;
    GET_VERTEX_FLAGS(ePosition | eBump | eUV | eUV2);
    BEGIN_INPUT_LAYOUT
    {
        LAYOUT_POSITION(0),
        LAYOUT_BUMP(12),
        LAYOUT_UV(56),
        LAYOUT_UV2(64),
    }
    END_INPUT_LAYOUT
};

uint32 GetVertexSize(uint32 aVertexFlags);
bool CreateInputLayout(CRenderManager &aRenderManager, uint32 aVertexFlags, ID3DBlob * aBlob, ID3D11InputLayout ** aVertexLayout);
uint32 GetFlagsFromString(const std::string& aString);
}

#endif //_ENGINE_VERTEXTYPES_20161229
