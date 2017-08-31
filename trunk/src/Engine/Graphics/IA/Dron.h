#pragma once
#ifndef _DRON_20170809
#define _DRON_20170809

#include "Graphics/Mesh/Mesh.h"
#include "Input/CharacterController.h"
#include "Graphics\IA\NavMeshManager.h"
#include "Graphics\IA\enemy.h"
#include "Math\Vector3.h"
#include "Render\RenderManager.h"
#include <string>


class CDron : public CEnemy
{

public:
    CDron(CXMLElement* aTreeNode);
    virtual ~CDron();

    GET_SET(float, SightDistance);
    GET_SET(float, MaxDetectDistance);
    GET_SET(float, DeadDistance);
    GET_SET(float, DetectAngle);
    GET_SET(float, speedPatroling);
    GET_SET(float, speedChasing);
    GET_SET(float, investigatingTolerance);
    GET_SET_PTR(CNavMesh, navMesh);

    void DrawImgui();
    std::vector<std::string> m_patrolPoints;
    CNavMesh *m_pnavMesh;
    float m_SightDistance;
    float m_MaxDetectDistance;
    float m_DeadDistance;
    float m_DetectAngle;
    std::string m_soundDetected;
    float m_speedPatroling;
    float m_speedChasing;
    float m_investigatingTolerance;

    void patrol() override;
    CCharacterController m_DronController;
    CPhysXManager &m_PhysXManager;
    Vect3f m_Movement;
    Vect3f m_Position;
    void GotoNextPoint();
    int m_DestPoint = 0;
    Vect3f GetPatrolPosition();
    Vect3f m_Destination;
    double distanceBetweenTwoPoints(double x, double y, double a, double b);
};

#endif //_DRON_20170809