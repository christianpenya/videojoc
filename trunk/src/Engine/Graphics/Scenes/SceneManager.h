#pragma once
#ifndef _ENGINE_SCENEMANAGER_20170110_H
#define _ENGINE_SCENEMANAGER_20170110_H

#include "Utils\TemplatedMapVector.h"
#include "Scene.h"

class CSceneManager : public base::utils::CTemplatedMapVector<CScene>
{
public:
    CSceneManager();
    virtual ~CSceneManager();
    bool Load(const std::string& aFilename);
    bool ReloadCurrentScene();

    bool Update(float elapsedTime);
    bool Render(const std::string& aLayer);
    void Activate(const std::string& aScene, bool aBool);
    std::vector<CScene*> GetScenes();
    CScene* GetCurrentScene();
    void DrawImgui();
private:
    bool Load();
protected:
    std::string m_Filename;
    void Destroy();
};

#endif //_ENGINE_SCENEMANAGER_20170110_H