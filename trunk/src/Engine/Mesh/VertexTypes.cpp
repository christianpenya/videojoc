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
    else if (aVertexFlags == PositionWeightIndicesNormalUV::GetVertexFlags())
    {
        return sizeof(PositionWeightIndicesNormalUV);
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
        VertexTypes::PositionNormalUV::CreateInputLayout(aRenderManager, aBlob, aVertexLayout);
        lOk = true;
    }
    else if (aVertexFlags == PositionUV::GetVertexFlags())
    {
        VertexTypes::PositionUV::CreateInputLayout(aRenderManager, aBlob, aVertexLayout);
        lOk = true;
    }
    else if (aVertexFlags == PositionBump::GetVertexFlags())
    {
        VertexTypes::PositionBump::CreateInputLayout(aRenderManager, aBlob, aVertexLayout);
        lOk = true;
    }
    else if (aVertexFlags == PositionBumpUV::GetVertexFlags())
    {
        VertexTypes::PositionBumpUV::CreateInputLayout(aRenderManager, aBlob, aVertexLayout);
        lOk = true;
    }
    else if (aVertexFlags == PositionBumpUVUV2::GetVertexFlags())
    {
        VertexTypes::PositionBumpUVUV2::CreateInputLayout(aRenderManager, aBlob, aVertexLayout);
        lOk = true;
    }
    else if (aVertexFlags == PositionWeightIndicesNormalUV::GetVertexFlags())
    {
        VertexTypes::PositionWeightIndicesNormalUV::CreateInputLayout(aRenderManager, aBlob, aVertexLayout);
        lOk = true;
    }

    if (!lOk)
        assert(!"Error creating input layout");

    return lOk;
}

uint32 GetFlagsFromString(const std::string& aString)
{
    uint32 lOut;

    if (aString == "PositionUV")
    {
        lOut = PositionUV::GetVertexFlags();
    }
    else if (aString == "PositionNormal")
    {
        lOut = PositionNormal::GetVertexFlags();
    }
    else if (aString == "PositionNormalUV")
    {
        lOut = PositionNormalUV::GetVertexFlags();
    }
    else if (aString == "PositionBump")
    {
        lOut = PositionBump::GetVertexFlags();
    }
    else if (aString == "PositionBumpUV")
    {
        lOut = PositionBumpUV::GetVertexFlags();
    }
    else if (aString == "PositionBumpUVUV2")
    {
        lOut = PositionBumpUVUV2::GetVertexFlags();
    }
    else if (aString == "PositionWeightIndicesNormalUV")
    {

        lOut = PositionWeightIndicesNormalUV::GetVertexFlags();

    }

    if (!lOut)
        assert(!"Wrong string flags!");

    return lOut;

}
}