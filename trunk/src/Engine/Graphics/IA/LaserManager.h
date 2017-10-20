#pragma once
#ifndef _LASERMANAGER_H_
#define _LASERMANAGER_H_

#include "Utils\TemplatedMapVector.h"
#include "Laser.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif


class CLaserManager : public base::utils::CTemplatedMapVector<CLaser>
{
public:
    CLaserManager();
    virtual ~CLaserManager();
    bool Load(const std::string& aFilename);
    void Reload();
    void DrawImgui(const char* title, bool* open);
private:
    std::string m_Filename;
};

#endif
