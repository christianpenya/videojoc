#pragma once
#ifndef _ENGINE_SCENENODE_20170110_H
#define _ENGINE_SCENENODE_20170110_H

#include "Base\Math\Transform.h"
#include "Base\Utils\Name.h"

class CSceneNode : public CTransform, public CName
{
public:
    CSceneNode();
    CSceneNode(const CXMLElement* aElement);
    virtual ~CSceneNode();
    virtual void Update(float aDeltaTime) {};
    virtual void Render() {};
    GET_SET_BOOL(Visible)
protected:
    bool m_Visible;
};

#endif //_ENGINE_SCENENODE_20170110_H