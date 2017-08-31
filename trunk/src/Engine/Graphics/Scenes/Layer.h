#pragma once
#ifndef _ENGINE_LAYER_20170110_H
#define _ENGINE_LAYER_20170110_H

#include "SceneNode.h"
#include "Utils/TemplatedMapVector.h"

class CScene;

class CLayer : public CName, public CActive, public base::utils::CTemplatedMapVector<CSceneNode>, public CTemplatedParent<CScene>
{
public:
    CLayer(const std::string& aName);
    virtual ~CLayer();
    bool Load(CXMLElement* aElement);
    bool Refresh();
    bool Update(float elapsedTime);
    bool Render();
    bool Render(CRenderManager& lRenderManager);
    std::string GetLayerName()
    {
        return m_Name;
    }

    std::vector<CSceneNode*> GetNodes();
    void DrawImgui();

private:
    DISALLOW_COPY_AND_ASSIGN(CLayer);

};

#endif //_ENGINE_LAYER_20170110_H