#pragma once
#ifndef _LASER_20170809
#define _LASER_20170809

#include "Graphics/Scenes/SceneNode.h"
#include "Math\Vector3.h"
#include "Render\RenderManager.h"
#include <string>
#include <random>
#include "Physx/PhysxManager.h"
#include "Graphics/Mesh/VertexTypes.h"
#include "Graphics/Mesh/TemplatedGeometry.h"
#include "Graphics/Materials/Material.h"
#include "XML\XML.h"
#include "Utils/Logger.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CLaser : public CSceneNode
{

public:
    CLaser();
    virtual ~CLaser();
    CLaser(CXMLElement* aTreeNode);

    struct CDebugVertex
    {
        Vect4f Position;
        CColor Color;
    };

    struct CUSTOMVERTEX
    {
        float x, y, z;
    };

    GET_SET(float, NormalSpeed);
    GET_SET(float, SprintSpeed);
    GET_SET(float, Timer);
    GET_SET(float, StartTime);
    GET_SET(float, Speed);
    GET_SET(Vect3f, StartPosition);
    GET_SET(Vect3f, EndPosition);

    std::string GetName()
    {
        return m_Name;
    }

    void DrawImgui();
    float m_NormalSpeed;
    float m_SprintSpeed;
    float m_Timer;
    Vect3f m_StartPosition;
    Vect3f m_GunPosition;
    Vect3f m_EndPosition;
    float m_StartTime;
    float m_Speed;
    float m_BoundMinX;
    float m_BoundMaxX;
    float m_BoundMinZ;
    float m_BoundMaxZ;
    float m_DeadDistance;
    bool m_Search;
    CPhysXManager::RaycastData* resultado;

    CPhysXManager &m_PhysXManager;
    CGeometry* mGeometries;
    CMaterial* mMaterials;
    bool Render(CRenderManager& lRM);


    bool Update(float ElapsedTime);
    void CalculateNextPositionLaser(float ElapsedTime);
    void FireLaser();
    float GetRandomValue(float min, float max);
};

#endif //_LASER_20170809