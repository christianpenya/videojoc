#pragma once
#ifndef _ENGINE_SCENEMESH_20170110_H
#define _ENGINE_SCENEMESH_20170110_H

#include "SceneNode.h"
#include "Utils/EnumToString.h"

class CMesh;
class CRenderManager;
class CSceneMesh : public CSceneNode
{
public:
    enum ERigidBody
    {
        ePlane = 0,
        eBox,
        eSphere,
        eShape,
        eTriangleMesh,
        ePlayer,
        eTriggerBox,
        eRigidBodyCount

    };

    explicit CSceneMesh(CXMLElement* aElement);
    CSceneMesh(CXMLElement* aElement, CMesh* aMesh);
    virtual ~CSceneMesh();
    bool Update(float aDeltaTime);
    virtual bool Render(CRenderManager& aRendermanager);
    void DrawImgui();
protected:
    CMesh* mMesh;
    CSceneMesh::ERigidBody mRigidBodyEnum;
private:
    Vect3f cubeOffset;
};

//--------------------------------------------------------------------------------------------------------
Begin_Enum_String(CSceneMesh::ERigidBody)
{
    Enum_String_Id(CSceneMesh::ePlane, "plane");
    Enum_String_Id(CSceneMesh::eBox, "box");
    Enum_String_Id(CSceneMesh::eSphere, "sphere");
    Enum_String_Id(CSceneMesh::eShape, "shape");
    Enum_String_Id(CSceneMesh::eTriangleMesh, "triangle_mesh");
    Enum_String_Id(CSceneMesh::ePlayer, "player");
    Enum_String_Id(CSceneMesh::eTriggerBox, "triggerBox");
}
End_Enum_String;

#endif //_ENGINE_SCENEMESH_20170110_H