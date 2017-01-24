#include "VertexTypes.h"

namespace VertexTypes
{
uint32 VertexTypes::GetVertexSize(uint32 aVertexFlags)
{
    if (aVertexFlags == PositionUV::GetVertexFlags())
    {
        return sizeof(PositionUV);
    }
    else if (aVertexFlags == PositionNormal::GetVertexFlags())
    {
        return sizeof(PositionNormal);
    }
    else if (aVertexFlags == PositionNormalUV::GetVertexFlags())
    {
        return sizeof(PositionNormalUV);
    }
    else if (aVertexFlags == PositionBump::GetVertexFlags())
    {
        return sizeof(PositionBump);
    }
    else if (aVertexFlags == PositionBumpUV::GetVertexFlags())
    {
        return sizeof(PositionBumpUV);
    }
    else if (aVertexFlags == PositionBumpUVUV2::GetVertexFlags())
    {
        return sizeof(PositionBumpUVUV2);
    }

    assert(!"Invalid vertex flags");

    return 0;
}

bool CreateInputLayout(CRenderManager &aRenderManager, uint32 aVertexFlags, ID3DBlob * aBlob, ID3D11InputLayout ** aVertexLayout)
{
    bool lOk = false;
    if (aVertexFlags == PositionNormal::GetVertexFlags())
    {
        VertexTypes::PositionNormal::CreateInputLayout(aRenderManager, aBlob, aVertexLayout);
        lOk = true;
    }
    else if (aVertexFlags == PositionNormalUV::GetVertexFlags())
    {
        VertexTypes::PositionNormalUV::GetVertexFlags();
        lOk = true;
    }
    else if (aVertexFlags == PositionUV::GetVertexFlags())
    {
        VertexTypes::PositionUV::GetVertexFlags();
        lOk = true;
    }
    else if (aVertexFlags == PositionBump::GetVertexFlags())
    {
        VertexTypes::PositionBump::GetVertexFlags();
        lOk = true;
    }
    else if (aVertexFlags == PositionBumpUV::GetVertexFlags())
    {
        VertexTypes::PositionBumpUV::GetVertexFlags();
        lOk = true;
    }
    else if (aVertexFlags == PositionBumpUVUV2::GetVertexFlags())
    {
        VertexTypes::PositionBumpUVUV2::GetVertexFlags();
        lOk = true;
    }

    assert(!"Error creating input layout");

    return lOk;
}

uint32 GetFlagsFromString(const std::string& aString)
{
    if (aString == "PositionUV")
    {
        return PositionUV::GetVertexFlags();
    }
    else if (aString == "PositionNormal")
    {
        return PositionNormal::GetVertexFlags();
    }
    else if (aString == "PositionNormalUV")
    {
        return PositionNormalUV::GetVertexFlags();
    }
    else if (aString == "PositionBump")
    {
        return PositionBump::GetVertexFlags();
    }
    else if (aString == "PositionBumpUV")
    {
        return PositionBumpUV::GetVertexFlags();
    }
    else if (aString == "PositionBumpUVUV2")
    {
        return PositionBumpUVUV2::GetVertexFlags();
    }

    assert(!"Wrong string flags!");

    return 0;

}
}