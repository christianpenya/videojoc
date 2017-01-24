#pragma once
#ifndef _ENGINE_LAYER_20170110_H
#define _ENGINE_LAYER_20170110_H

#include "SceneNode.h"
#include "Utils\TemplatedMapVector.h"

class CLayer : public CName, public base::utils::CTemplatedMapVector< CSceneNode >
{
public:
    CLayer(const std::string& aName);
    virtual ~CLayer();
    bool Load(CXMLElement* aElement);
    bool Update(float elapsedTime);
    bool Render();
protected:
private:
    DISALLOW_COPY_AND_ASSIGN(CLayer);
};

#endif //_ENGINE_LAYER_20170110_H