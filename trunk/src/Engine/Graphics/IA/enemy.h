#pragma once
#ifndef _ENEMY_20170820_H
#define _ENEMY_20170820_H

#include <memory>
#include "Utils\TemplatedMap.h"
#include "Graphics/Mesh/Mesh.h"
#include <iostream>

#include "Utils/TemplatedMapVector.h"
#include "Math/Color.h"
#include "Utils/StringUtils.h"
//#include "Graphics/Scenes/SceneNode.h"
#include "Graphics/Scenes/SceneMesh.h"
#include "Utils/EnumToString.h"
#include "Render/RenderPipeline/DynamicTexture.h"
#include "Graphics/Scenes/Layer.h"
#include "Graphics\IA\NavMeshManager.h"
#include "Pathfinding.h"


using namespace std;
enum Input { PATROL, CHASE, STOP};

class CEnemy;
class CPatrolingState;
class CChasingState;
class CEnemyState
{
public:
    static std::shared_ptr<CPatrolingState> patroling;
    static std::shared_ptr<CChasingState> chasing;
    virtual ~CEnemyState() = default;
    virtual void handleInput(CEnemy&, Input) = 0;
    virtual void update(CEnemy&) = 0;
};

class CPatrolingState : public CEnemyState
{
public:
    virtual void handleInput(CEnemy&, Input) override;
    virtual void update(CEnemy&) override;
};

class CChasingState : public CEnemyState
{
public:
    virtual void handleInput(CEnemy&, Input) override;
    virtual void update(CEnemy&) override;
};


class CEnemy : public CSceneMesh
{

private:
    //std::string name;
    std::shared_ptr<CEnemyState> state;

    virtual void updateWithNewState()
    {
        state->update(*this);
    }

public:
    enum EEnemyType
    {
        eDron = 0,
        eGuard = 1
    };

    CEnemy(CXMLElement* aElement, EEnemyType aEnemyType);
    virtual ~CEnemy();
    CPhysXManager &m_PhysXManager;
    CPhysXManager::RaycastData* resultado;
    float m_SightDistance;
    float m_MaxDetectDistance;
    float m_DeadDistance;
    Vect3f m_Destination;
    Vect3f m_DestinationChase;
    CNavMesh *m_pnavMesh;
    CPathfinding *m_ppath;
    int m_DestPoint = 0;
    int m_DestPointChase = 0;
    bool m_Calculated;
    Vect3f m_Movement;
    Vect3f m_Position;
    bool m_CalculateReturn;
    std::vector<std::string> m_patrolPoints;
    Input m_State;
    float m_DetectAngle;


    GET_SET(float, SightDistance);
    GET_SET(float, MaxDetectDistance);
    GET_SET(float, DeadDistance);
    GET_SET(float, DetectAngle);

    GET_SET_PTR(CNavMesh, navMesh);
    std::vector<Vect3f> m_pathChasing;
    std::vector<Vect3f> m_pathChasingAux;


    GET_SET(EEnemyType, EnemyType);
    //CMesh* mMesh;
    std::string getName()
    {
        return m_Name;
    }

    EEnemyType m_EnemyType;
    void DrawImgui();
    bool Update(float ElapsedTime);
    double distanceBetweenTwoPoints(double x, double y, double a, double b);
    Vect3f GetPatrolPosition();
    bool PlayerOnSight();

    float getAngle(float x1, float y1, float z1, float x2, float y2, float z2);
    float getAngle2(float x1, float y1, float z1, float x2, float y2, float z2);

    virtual void handleInput(Input input)
    {
        state->handleInput(*this, input);
    }
    void changeState(std::shared_ptr<CEnemyState> newState)
    {
        state = newState;
        updateWithNewState();
    }
    virtual void patrol()
    {
        std::cout << GetName() << "patrol" << std::endl;
    }
    virtual void chase()
    {
        std::cout << GetName() << "chase" << std::endl;
    }

};

Begin_Enum_String(CEnemy::EEnemyType)
{
    Enum_String_Id(CEnemy::eDron, "dron");
    Enum_String_Id(CEnemy::eGuard, "guard");
}
End_Enum_String;

#endif