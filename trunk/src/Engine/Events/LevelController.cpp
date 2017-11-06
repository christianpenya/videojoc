#include "LevelController.h"
#include "Input/CharacterController.h"
#include "Utils/CheckedDelete.h"
#include "Graphics/Camera/TpsCameraController.h"
#include "Render/RenderManager.h"
#include"Graphics/Cinematics/CinematicsManager.h"
#include <concrt.h>

CLevelController::CLevelController()
{

    m_Engine = nullptr;
    m_SceneManager = nullptr;
    m_PhysxManager = nullptr;
    m_ActionManager = nullptr;
    m_TimePaused = true;

}

CLevelController::CLevelController(int lvl)
{
    Level = lvl;
    m_Engine = nullptr;
    m_SceneManager = nullptr;
    m_PhysxManager = nullptr;
    m_ActionManager = nullptr;
    m_TimePaused = true;
    m_PlayerDetected = false;
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

void CLevelController::PlayerDetected()
{
    m_PlayerDetected = true;

}



void CLevelController::Update(float elapsedTime)
{
    if ((*m_ActionManager)("pause")->value>0.1f&&!m_TimePaused)
    {
        PauseGame();

    }
    if (m_PlayerDetected && m_TimerDetected<5.0f)
    {
        m_TimerDetected += elapsedTime;
        CGUIManager* guiMan = &CEngine::GetInstance().GetGUIManager();
        Vect2f lPos;
        Vect2u lSize = CEngine::GetInstance().GetRenderManager().GetWindowSize();
        lPos.x = lSize.x / 2;
        lPos.y = lSize.y / 2;
        guiMan->FillCommandQueueWithText("font1", "Protagonista Descubierta", lPos, CGUIManager::MID_CENTER, CColor(.0f, .0f, .0f));
    }
    else
    {
        m_TimerDetected = 0;
        m_PlayerDetected = false;
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

void CLevelController::NewGame()
{
    if (m_SceneManager->GetCurrentScene()->Exist("MenuPrincipal"))
        m_SceneManager->GetCurrentScene()->GetLayer("MenuPrincipal")->SetActive(false);
    CEvent* event = CEngine::GetInstance().GetEventManager().GetEvent("Cinematica01");
    if (event)
        event->Start();
    event = CEngine::GetInstance().GetEventManager().GetEvent("c11");
    if (event)
        event->Start();
    event = CEngine::GetInstance().GetEventManager().GetEvent("mueveteWASD");
    if (event)
        event->Start();
    event = CEngine::GetInstance().GetEventManager().GetEvent("puertaSala0");
    if (event)
        event->Start();

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
            cam->SetYaw(-m_LastCheckpointR.GetYaw()+3.14);
        }
    }
}


void CLevelController::ToMainMenu()
{
    m_TimePaused = true;
    if (m_SceneManager->GetCurrentScene()->Exist("MenuPrincipal"))
        m_SceneManager->GetCurrentScene()->GetLayer("MenuPrincipal")->SetActive(true);
    if (m_SceneManager->GetCurrentScene()->Exist("PauseMENU"))
        m_SceneManager->GetCurrentScene()->GetLayer("PauseMENU")->SetActive(false);
    if (m_SceneManager->GetCurrentScene()->Exist("MenuMuerte"))
        m_SceneManager->GetCurrentScene()->GetLayer("MenuMuerte")->SetActive(false);
    if (m_SceneManager->GetCurrentScene()->Exist("MenuCompletado"))
        m_SceneManager->GetCurrentScene()->GetLayer("MenuCompletado")->SetActive(false);
}

void CLevelController::GameComplete()
{
    m_TimePaused = true;
    if (m_SceneManager->GetCurrentScene()->Exist("MenuCompletado"))
        m_SceneManager->GetCurrentScene()->GetLayer("MenuCompletado")->SetActive(true);
}

