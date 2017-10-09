#pragma once

#ifndef _LevelController_
#define _LevelController_


#include "Engine/Engine.h"

class CLevelController
{

public:

    CLevelController();
    CLevelController(int lvl);
    ~CLevelController();
    void Init();
    void PlayerDetected(std::string detectorName );  //Nombre del dron/guardia


private:
    int Level = 1;
    CEngine* m_Engine;
    CSceneManager* m_SceneManager;
    CPhysXManager* m_PhysxManager;


};





#endif