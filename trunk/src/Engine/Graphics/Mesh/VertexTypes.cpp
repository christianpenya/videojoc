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
    else if (aVertexFlags == PositionNormalUVUV2::GetVertexFlags())
    {
        return sizeof(PositionNormalUVUV2);
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
    else if (aVertexFlags == ParticleVertex::GetVertexFlags())
    {
        return sizeof(ParticleVertex);
    }
    else if (aVertexFlags == SpriteVertex::GetVertexFlags())
    {
        return sizeof(SpriteVertex);
    }

#ifdef _DEBUG
    assert(!"Invalid vertex flags");
#endif

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
    else if (aVertexFlags == PositionNormalUVUV2::GetVertexFlags())
    {
        VertexTypes::PositionNormalUVUV2::CreateInputLayout(aRenderManager, aBlob, aVertexLayout);
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
    else if (aVertexFlags == ParticleVertex::GetVertexFlags())
    {
        VertexTypes::ParticleVertex::CreateInputLayout(aRenderManager, aBlob, aVertexLayout);
        lOk = true;
    }
    else if (aVertexFlags == SpriteVertex::GetVertexFlags())
    {
        VertexTypes::SpriteVertex::CreateInputLayout(aRenderManager, aBlob, aVertexLayout);
        lOk = true;
    }


#ifdef _DEBUG
    if (!lOk)
        assert(!"Error creating input layout");
#endif

    return lOk;
}

uint32 GetFlagsFromString(const std::string& aString)
{
    uint32 lOut = 0;

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
    else if (aString == "PositionNormalUVUV2")
    {
        lOut = PositionNormalUVUV2::GetVertexFlags();
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
    else if (aString == "ParticleVertex")
    {

        lOut = ParticleVertex::GetVertexFlags();

    }
    else if (aString == "SpriteVertex")
    {

        lOut = SpriteVertex::GetVertexFlags();

    }


#ifdef _DEBUG
    if (!lOut)
        assert(!"Wrong string flags!");
#endif

    return lOut;

}
}