#pragma once
#ifndef _ENEMYANIMATED_20170820_H
#define _ENEMYANIMATED_20170820_H

#include "Graphics/Scenes/SceneMesh.h"
#include "Graphics/Animation/SceneAnimatedModel.h"
#include "Graphics/Scenes/Layer.h"
#include "Graphics\IA\NavMeshManager.h"
#include "Pathfinding.h"
#include "Physx/PhysxManager.h"
#include "Graphics/IA/Enemy.h"


using namespace std;
enum Input { PATROL, CHASE, STOP };

class CEnemyAnimated;
class CPatrolingState;
class CChasingState;
class CEnemyState
{
public:
    static std::shared_ptr<CPatrolingState> patroling;
    static std::shared_ptr<CChasingState> chasing;
    virtual ~CEnemyState() = default;
    virtual void handleInput(CEnemyAnimated&, Input) = 0;
    virtual void update(CEnemyAnimated&) = 0;
};

class CPatrolingState : public CEnemyState
{
public:
    virtual void handleInput(CEnemyAnimated&, Input) override;
    virtual void update(CEnemyAnimated&) override;
};

class CChasingState : public CEnemyState
{
public:
    virtual void handleInput(CEnemyAnimated&, Input) override;
    virtual void update(CEnemyAnimated&) override;
};



class CEnemyAnimated : public CEnemy, public CSceneAnimatedModel
{

private:
    //std::string name;
    std::shared_ptr<CEnemyState> state;

    virtual void updateWithNewState()
    {
        state->update(*this);
    }

public:
    CEnemyAnimated(CXMLElement* aElement);
    virtual ~CEnemyAnimated();
    float m_SightDistance;
    float m_MaxDetectDistance;
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
    int m_Group;
    Vect3f m_Height = Vect3f(0.f, 0.0f, 0.0f); //Vect3f(0.f, 0.0f, .0f); Para guardias
    std::string m_Corename;
    CSceneAnimatedModel* m_SceneAnimatedModel;


    GET_SET(float, SightDistance);
    GET_SET(float, MaxDetectDistance);
    GET_SET(float, DetectAngle);
    GET_SET(std::string, Corename);


    GET_SET_PTR(CNavMesh, navMesh);
    std::vector<Vect3f> m_pathChasing;
    std::vector<Vect3f> m_pathChasingAux;

    void DrawImgui();
    bool Update(float ElapsedTime);
    Vect3f GetPatrolPosition();
    bool PlayerOnSight();
    float m_ElapsedTime;

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
        //  std::cout << GetName() << "patrol" << std::endl;
    }
    virtual void chase()
    {
        //    std::cout << GetName() << "chase" << std::endl;
    }

};


#endif