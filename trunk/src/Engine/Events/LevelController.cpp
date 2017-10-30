#include "LevelController.h"
#include "Input/CharacterController.h"
#include "Utils/CheckedDelete.h"
#include "Graphics/Camera/TpsCameraController.h"

CLevelController::CLevelController()
{

    m_Engine = nullptr;
    m_SceneManager = nullptr;
    m_PhysxManager = nullptr;
    m_ActionManager = nullptr;
    m_TimePaused = false;

}

CLevelController::CLevelController(int lvl)
{
    Level = lvl;
    m_Engine = nullptr;
    m_SceneManager = nullptr;
    m_PhysxManager = nullptr;
    m_ActionManager = nullptr;
    m_TimePaused = false;
}

CLevelController::~CLevelController()
{
    base::utils::CheckedDelete(m_Engine);
    base::utils::CheckedDelete(m_SceneManager);
    base::utils::CheckedDelete(m_PhysxManager);
    base::utils::CheckedDelete(m_ActionManager);
}

void CLevelController::Init()
{
    m_Engine = &CEngine::GetInstance();
    m_SceneManager = &m_Engine->GetSceneManager();
    m_PhysxManager = &m_Engine->GetPhysXManager();
    m_ActionManager = &m_Engine->GetActionManager();

}

void CLevelController::PlayerDetected(std::string detectorName)
{
    std::string l_Nombre = detectorName;
    //Ejemplo: Mostrar indicador UI del posicionamiento del detector respecto la prota
}



void CLevelController::Update(float elapsedTime)
{
    if ((*m_ActionManager)("pause")->value>0.1f&&Level!=0&&!m_TimePaused)
    {
        PauseGame();

    }

}


void CLevelController::PauseGame()
{
    m_TimePaused = true;
    if (m_SceneManager->GetCurrentScene()->Exist("PauseMENU"))
        m_SceneManager->GetCurrentScene()->GetLayer("PauseMENU")->SetActive(true);
}
void CLevelController::ResumeGame()
{
    m_TimePaused = false;
    if (m_SceneManager->GetCurrentScene()->Exist("PauseMENU"))
        m_SceneManager->GetCurrentScene()->GetLayer("PauseMENU")->SetActive(false);
}

void CLevelController::RestoreLastCheckpoint()
{
    CCharacterController* charContr = (CCharacterController*)CEngine::GetInstance().m_CharacterController;
    if (charContr != nullptr)
    {
        charContr->moveToLastCheckpoint(m_LastCheckpointP, m_LastCheckpointR);

    }
    CCameraManager* camMan = (CCameraManager*)&CEngine::GetInstance().GetCameraManager();
    if (camMan)
    {
        CTpsCameraController* cam =(CTpsCameraController*) &camMan->GetCurrentCamera();
        if (cam)
        {
            cam->SetPitch(0);
            cam->SetYaw(m_LastCheckpointR.GetYaw()+3.1415f);
        }
    }
}