#pragma once
#ifndef _ENGINE_TECHNIQUEPOOLMANAGER_20170104
#define _ENGINE_TECHNIQUEPOOLMANAGER_20170104

#include "Technique.h"
#include "TechniquePool.h"
#include "Utils\TemplatedMapVector.h"

class CTechniquePoolManager : public base::utils::CTemplatedMapVector<CTechnique>
{
public:
    CTechniquePoolManager();
    virtual ~CTechniquePoolManager();
    bool Load(const std::string& aFilename);
    bool Reload();
    bool Apply(const std::string& aPoolName);
private:
    DISALLOW_COPY_AND_ASSIGN(CTechniquePoolManager);
    std::string m_Filename;
    typedef base::utils::CTemplatedMapVector<CTechniquePool> TMapPools;
    TMapPools mPools;
};

#endif //_ENGINE_TECHNIQUEPOOLMANAGER_20170104
