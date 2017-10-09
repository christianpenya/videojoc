#include "LevelController.h"
#include "Utils/CheckedDelete.h"

CLevelController::CLevelController()
{
    m_Engine = nullptr;
    m_SceneManager = nullptr;
    m_PhysxManager = nullptr;
}

CLevelController::CLevelController(int lvl)
{
    Level = lvl;
    m_Engine = nullptr;
    m_SceneManager = nullptr;
    m_PhysxManager = nullptr;
}

CLevelController::~CLevelController()
{
    base::utils::CheckedDelete(m_Engine);
    base::utils::CheckedDelete(m_SceneManager);
    base::utils::CheckedDelete(m_PhysxManager);
}

void CLevelController::Init()
{
    m_Engine = &CEngine::GetInstance();
    m_SceneManager = &m_Engine->GetSceneManager();
    m_PhysxManager = &m_Engine->GetPhysXManager();

}

void CLevelController::PlayerDetected(std::string detectorName)
{
    std::string l_Nombre = detectorName;
    //Ejemplo: Mostrar indicador UI del posicionamiento del detector respecto la prota
}




