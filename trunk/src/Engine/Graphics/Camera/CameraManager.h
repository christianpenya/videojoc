#pragma once
#ifndef _ENGINE_CAMERAMANAGER_CPB_20170810_H
#define _ENGINE_CAMERAMANAGER_CPB_20170810_H

#include "Utils/Defines.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

#include "Graphics/Camera/SphericalCameraController.h"
#include "Graphics/Camera/FpsCameraController.h"
#include "Graphics/Camera/TpsCameraController.h"
#include "Graphics/Camera/FreeCameraController.h"

class CCameraManager
{
public:
    CCameraController* m_CurrentCamera;

    CFreeCameraController* m_FreeCam;
    CFpsCameraController* m_FpsCam;
    CSphericalCameraController* m_OrbitalCam;
    CTpsCameraController* m_TPSCam;
    int m_CameraSelector;
    int m_PrevCameraSelector;

    CCameraManager();
    ~CCameraManager();

    CCameraController& GetCurrentCamera();
    void SetCurrentCamera(CCameraController* aCurrentCamera);

    void Init(CCharacterController* aCharacterController);
    void Update(float ElapsedTime);
    void SwitchCamera();
};

#endif //_ENGINE_CAMERAMANAGER_CPB_20170810_H
