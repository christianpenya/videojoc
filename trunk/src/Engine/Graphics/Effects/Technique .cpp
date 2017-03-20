#include "Technique.h"

CTechnique::CTechnique():
    mEffect(nullptr)
{}

CTechnique::~CTechnique()
{
    mEffect = nullptr;
}

void CTechnique::SetEffect(CEffect* aEffect)
{
    mEffect = aEffect;
}

CEffect* CTechnique::GetEffect() const
{
    return mEffect;
}

void CTechnique::Bind(ID3D11DeviceContext* aContext)
{
    if (mEffect!=nullptr)
        mEffect->Bind(aContext);
}