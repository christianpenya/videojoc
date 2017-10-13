#pragma once
#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_

#include "Utils\TemplatedMapVector.h"
#include "ParticleSystemType.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif


class CParticleManager : public base::utils::CTemplatedMapVector<CParticleSystemType>
{
public:
    CParticleManager();
    virtual ~CParticleManager();
    bool Load(const std::string& aFilename);
    void Reload();
    void DrawImgui();
private:
    std::string m_Filename;
};

#endif
