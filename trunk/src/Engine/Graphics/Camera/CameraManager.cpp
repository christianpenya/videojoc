#include "CameraManager.h"

CCameraManager::CCameraManager():
    m_CameraSelector(0),
    m_PrevCameraSelector(-1)
{
    m_FreeCam = new CFreeCameraController(Vect3f(-10.0f, 2.36f, 5.0f), Vect3f(1, 0, 0), Vect3f(0, 1, 0), Vect4f(1, 500, 1.13f, 1.7f), 1.5f, -1.5f, 10.0f, -10.0f);
    m_FpsCam = new CFpsCameraController(Vect3f(0, 1, 0), 1.5f, -1.5f);
    m_OrbitalCam = new CSphericalCameraController();
    m_TPSCam = new CTpsCameraController();
}

CCameraManager::~CCameraManager()
{
    base::utils::CheckedDelete(m_FreeCam);
    base::utils::CheckedDelete(m_FpsCam);
    base::utils::CheckedDelete(m_OrbitalCam);
}

void CCameraManager::SetCurrentCamera(CCameraController* aCurrentCamera)
{
    m_CurrentCamera = aCurrentCamera;
}

CCameraController& CCameraManager::GetCurrentCamera()
{
    return *m_CurrentCamera;
}

void CCameraManager::Init(CCharacterController* aCharacterController)
{
    m_TPSCam->Init(aCharacterController);
    m_CurrentCamera = m_TPSCam;
}

void CCameraManager::Update(float ElapsedTime)
{

    if (m_CameraSelector != m_PrevCameraSelector)
    {
        SwitchCamera();
    }

    m_CurrentCamera->Update(ElapsedTime);
}

void CCameraManager::SwitchCamera()
{
    m_PrevCameraSelector = m_CameraSelector;

    switch (m_CameraSelector)
    {
    case 0:
        m_CurrentCamera = m_TPSCam;
        break;
    case 1:
        m_CurrentCamera = m_FreeCam;
        break;
    default:
        break;
    }
}
