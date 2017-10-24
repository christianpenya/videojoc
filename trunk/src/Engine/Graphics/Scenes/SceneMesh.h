#pragma once
#ifndef _ENGINE_SCENEMESH_20170110_H
#define _ENGINE_SCENEMESH_20170110_H

#include "SceneNode.h"
#include "Utils/EnumToString.h"
#include "Physx/PhysxManager.h"
#include "Sound/ISoundManager.h"

class CMesh;
class CRenderManager;

class CSceneMesh : public CSceneNode
{
public:

    enum ERigidBody
    {
        ePlane = 0,
        eBox,
        eDynamicBox,
        eSphere,
        eShape,
        eTriangleMesh,
        ePlayer,
        eEnemy,
        eTriggerBox,
        eRigidBodyCount
    };

    explicit CSceneMesh(CXMLElement* aElement);
    CSceneMesh(CXMLElement* aElement, CMesh* aMesh);
    virtual ~CSceneMesh();
    void Initialize(CXMLElement* aElement);
    bool Update(float aDeltaTime);
    virtual bool Render(CRenderManager& aRendermanager);

    void DrawImgui();
    void Deactivate();

    bool HasGotPhysx();
    void DeletePhysx();

protected:
    CMesh* mMesh;
    CSceneMesh::ERigidBody mRigidBodyEnum;

private:
    size_t mPhysxIndex;
    Vect3f mCubeOffset;

    SoundEvent mSoundEvent;
    float mAudioPeriod;
    float mTimeAcum;
};

//--------------------------------------------------------------------------------------------------------
Begin_Enum_String(CSceneMesh::ERigidBody)
{
    Enum_String_Id(CSceneMesh::ePlane, "plane");
    Enum_String_Id(CSceneMesh::eBox, "box");
    Enum_String_Id(CSceneMesh::eDynamicBox, "dynamic_box");
    Enum_String_Id(CSceneMesh::eSphere, "sphere");
    Enum_String_Id(CSceneMesh::eShape, "shape");
    Enum_String_Id(CSceneMesh::eTriangleMesh, "triangle_mesh");
    Enum_String_Id(CSceneMesh::ePlayer, "player");
    Enum_String_Id(CSceneMesh::eEnemy, "enemy");
    Enum_String_Id(CSceneMesh::eTriggerBox, "triggerBox");
}
End_Enum_String;

#endif //_ENGINE_SCENEMESH_20170110_H