#pragma once
#ifndef _ENGINE_SCENE_20170110_H
#define _ENGINE_SCENE_20170110_H

#include "Utils\Name.h"
#include "Layer.h"

class CScene : public CName, public base::utils::CTemplatedMapVector< CLayer >
{
public:
    CScene(const std::string& aName);
    virtual ~CScene();
    bool Load(const std::string& aFilename);
    bool Update(float elapsedTime);
    bool Render();
    bool Render(const std::string& aLayerName);
    GET_SET_BOOL(Active)
    std::vector<CLayer*> GetLayers();
    void DrawImGui();
protected:
    bool m_Active;
private:
    DISALLOW_COPY_AND_ASSIGN(CScene);
};

#endif //_ENGINE_SCENE_20170110_H