#pragma once
#ifndef _ENGINE_SCENEMESH_20170110_H
#define _ENGINE_SCENEMESH_20170110_H

#include "SceneNode.h"

class CMesh;
class CRenderManager;
class CSceneMesh : public CSceneNode
{
public:
    explicit CSceneMesh(CXMLElement* aElement);
    CSceneMesh(CXMLElement* aElement, CMesh* aMesh);
    virtual ~CSceneMesh();
    bool Update(float aDeltaTime);
    virtual bool Render(CRenderManager& aRendermanager);
    void DrawImgui();
protected:
    CMesh* mMesh;
};

#endif //_ENGINE_SCENEMESH_20170110_H