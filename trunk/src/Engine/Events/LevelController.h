#pragma once

#ifndef _LevelController_
#define _LevelController_
#include "Utils/Defines.h"

#include "Engine/Engine.h"
#include "Input/ActionManager.h"
#include "Graphics/Scenes/SceneManager.h"
#include "GUI/GUIManager.h"

#include "Math/Quaternion.h"
#include "Graphics/Camera/CameraManager.h"


class CLevelController
{

public:

    CLevelController();
    CLevelController(int lvl);
    ~CLevelController();
    void Init();
    void PlayerDetected();

    void Update(float elapsedTime);
    void PauseGame();
    void ResumeGame();
    GET_SET(bool, TimePaused);
    GET_SET(Vect3f, LastCheckpointP);
    GET_SET(Quatf, LastCheckpointR);
    void RestoreLastCheckpoint();
    Vect3f m_LastCheckpointP = Vect3f(0, 0, 0);
    Quatf m_LastCheckpointR = Quatf(0, 0, 0, 1);
private:
    int Level = 1;
    CEngine* m_Engine;
    CSceneManager* m_SceneManager;
    CPhysXManager* m_PhysxManager;
    CActionManager* m_ActionManager;
    bool m_TimePaused;
    bool m_PlayerDetected;
    float m_TimerDetected;




};





#endif