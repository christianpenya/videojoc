#pragma once
#ifndef _ENGINE_SCENENODE_20170110_H
#define _ENGINE_SCENENODE_20170110_H

#include "Math\Transform.h"
#include "Utils\Name.h"

class CRenderManager;

class CSceneNode : public CTransform, public CName
{
public:
    CSceneNode();
    CSceneNode(const CXMLElement* aElement);
    virtual ~CSceneNode();
    virtual bool Update(float aDeltaTime);
    virtual bool Render(CRenderManager& aRendermanager);
    GET_SET_BOOL(Visible)
protected:
    bool m_Visible;
};

#endif //_ENGINE_SCENENODE_20170110_H