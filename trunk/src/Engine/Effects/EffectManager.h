#pragma once
#ifndef _ENGINE_EFFECT_MANAGER_20170104
#define _ENGINE_EFFECT_MANAGER_20170104

#include "Base\Utils\TemplatedMapVector.h"
#include "Effect.h"

class CEffectManager : public base::utils::CTemplatedMapVector<CEffect>
{
public:
    CEffectManager();
    virtual ~CEffectManager();
    bool Load(const std::string& aFilename);
    bool Reload();

private:
    DISALLOW_COPY_AND_ASSIGN(CEffectManager);
    std::string mFilename;
};

#endif //_ENGINE_EFFECT_MANAGER_20170104
