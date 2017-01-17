#pragma once
#ifndef _ENGINE_SCENEMANAGER_20170110_H
#define _ENGINE_SCENEMANAGER_20170110_H

#include "Base\Utils\TemplatedMapVector.h"
#include "Scene.h"

class CSceneManager : public base::utils::CTemplatedMapVector<CScene>
{
public:
    CSceneManager();
    virtual ~CSceneManager();
    bool Load(const std::string& aFilename);
    bool Update(float elapsedTime);
    bool Render(const std::string& aLayer);
    bool Reload();
    void Activate(const std::string& aScene, bool aBool);
private:
    bool Load();
protected:
    std::string m_Filename;
    void Destroy();
};

#endif //_ENGINE_SCENEMANAGER_20170110_H