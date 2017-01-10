#pragma once
#ifndef _ENGINE_TECHNIQUE_20170107
#define _ENGINE_TECHNIQUE_20170107

#include "Effect.h"

class CTechnique
{
public:
    CTechnique();
    virtual ~CTechnique();
    void SetEffect(CEffect* aEffect);
    CEffect* GetEffect() const;
    void Bind(ID3D11DeviceContext* aContext);
private:
    DISALLOW_COPY_AND_ASSIGN(CTechnique);
    CEffect* mEffect;
};

#endif //_ENGINE_TECHNIQUE_20170107
