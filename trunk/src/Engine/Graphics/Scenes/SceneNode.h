#pragma once
#ifndef _ENGINE_SCENENODE_20170110_H
#define _ENGINE_SCENENODE_20170110_H

#include "Math/Transform.h"
#include "Utils/Name.h"
#include "Utils/Active.h"
#include "Utils/TemplatedParent.h"
#include "Graphics/Mesh/BoundingSphere.h"

class CRenderManager;
class CLayer;

class CSceneNode : public CTransform, public CName, public CActive, public CTemplatedParent<CLayer>
{
public:

    enum ESceneNodeType
    {
        eMesh,
        eAnimatedModel,
        eBasicPrimitive,
        eLight,
        eParticle,
        eSceneNodeCount
    };

    CSceneNode();
    CSceneNode(const CXMLElement* aElement);
    virtual ~CSceneNode();

    virtual bool Update(float aDeltaTime);
    virtual bool Render(CRenderManager& lRM);

    GET_SET_BOOL(Visible);
    GET_SET(ESceneNodeType, NodeType);

    virtual void DrawImgui();

protected:
    bool m_ignoreFrustum;
    bool m_Visible;
    ESceneNodeType m_NodeType;
    CBoundingSphere mBS;
};

#endif //_ENGINE_SCENENODE_20170110_H
