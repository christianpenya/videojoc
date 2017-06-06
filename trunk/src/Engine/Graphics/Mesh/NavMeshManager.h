#pragma once
#ifndef _NAVMESHMANAGER_H_
#define _NAVMESHMANAGER_H_

#include "Utils\TemplatedMapVector.h"
#include "NavMesh.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif


class CNavMeshManager : public base::utils::CTemplatedMapVector<CNavMesh>
{
public:
    CNavMeshManager();
    virtual ~CNavMeshManager();
    bool Load(const std::string& aFilename);
    void Reload();
    void DrawImgui(const char* title, bool* open);
private:
    std::string m_Filename;
};

#endif
