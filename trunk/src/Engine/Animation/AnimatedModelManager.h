#pragma once
#ifndef _ENGINE_ANIMATEDMODELMANAGER_20170117
#define _ENGINE_ANIMATEDMODELMANAGER_20170117
#include "Utils\TemplatedMap.h"
#include "AnimatedCoreModel.h"

class CAnimatedCoreModel;
class CAnimatedModelManager : public base::utils::CTemplatedMap<CAnimatedCoreModel>
{
private:
    std::string m_Filename;
public:
    CAnimatedModelManager();
    virtual ~CAnimatedModelManager();
    bool Load(const std::string &Filename);
    void Reload();
};

#endif //_ENGINE_ANIMATEDMODELMANAGER_20170117
