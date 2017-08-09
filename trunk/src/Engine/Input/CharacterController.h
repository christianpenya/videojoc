#pragma once
#ifndef _ENGINE_CHARACTERCONTROLLER_20170310_H
#define _ENGINE_CHARACTERCONTROLLER_20170310_H

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif
#include "Math/Vector3.h"
#include "ActionManager.h"
#include "Graphics/Scenes/SceneManager.h"
//#include "Physx/PhysxManager.h"
class CPhysXManager;
class CCharacterController
{

public:
    CCharacterController() :
        m_Movement(0, 0, 0),
        m_Position(0,0,0),
        m_Speed(.5f),
        player(nullptr)
    {



    }

    virtual ~CCharacterController() {}

    virtual void Update(float ElapsedTime);
    virtual void Init(CSceneManager* sceneManager);
    Vect3f m_Movement;
    Vect3f m_Position;
    float m_Speed;
    CActionManager* actionManager;
    CSceneNode* player;

    CPhysXManager* physXManager;
private:
    Vect3f m_Gravity = (0.0f, 9.81f, 0.0f);
};

#endif //_ENGINE_CAMERACONTROLLER_CPB_20161126_H
