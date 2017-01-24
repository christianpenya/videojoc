#include "Technique.h"

CTechnique::CTechnique() {}
CTechnique::~CTechnique() {}

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
    mEffect->Bind(aContext);
}