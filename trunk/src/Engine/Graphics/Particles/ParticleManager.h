#pragma once
#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_

#include "Utils\TemplatedMapVector.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#include "ParticleSystemType.h"
#endif


class CParticleManager : public base::utils::CTemplatedMapVector<CParticleSystemType>
{
public:
    CParticleManager();
    virtual ~CParticleManager();
    bool Load(const std::string& aFilename);
    void Reload();
    void DrawImgui(const char* title, bool* open);
private:
    std::string m_Filename;
};

#endif
