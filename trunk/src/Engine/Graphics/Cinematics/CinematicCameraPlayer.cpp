#include "CinematicCameraPlayer.h"
#include "CinematicCameraKey.h"
#include "Engine/Engine.h"
#include "XML/tinyxml2/tinyxml2.h"
#include "Graphics/Camera/CinematicCameraController.h"
#include "Utils/Logger.h"

CCinematicCameraPlayer::CCinematicCameraPlayer() : m_CurrentCinematicCamera(nullptr), m_lastCameraState(nullptr) {}
CCinematicCameraPlayer::~CCinematicCameraPlayer()
{
    base::utils::CheckedDelete(m_CurrentCinematicCamera);
}

bool CCinematicCameraPlayer::Load(const CXMLElement *aElement)
{
    bool lOk = false;
    mName = aElement->GetAttribute<std::string>("name", "");

    for (tinyxml2::XMLElement const *iCamKey = aElement->FirstChildElement(); iCamKey != nullptr; iCamKey = iCamKey->NextSiblingElement())
    {
        if (strcmp(iCamKey->Name(), "key")==0)
        {
            CCinematicCameraKey *lCamKey = new CCinematicCameraKey();
            lOk = false || lCamKey->Load(iCamKey);
            Add(Convert(lCamKey->GetTime()),lCamKey);

        }
    }

    m_CurrentCinematicCamera = new CCinematicCameraController();

    return lOk;
}

void CCinematicCameraPlayer::Apply(float aPercentage, CCinematicKey* A, CCinematicKey* B)
{
    CCinematicCameraKey* a = dynamic_cast<CCinematicCameraKey*>(A);
    CCinematicCameraKey* b = dynamic_cast<CCinematicCameraKey*>(B);

    Vect3f lPos = m_CurrentCinematicCamera->GetPosition();
    Vect3f lUp = m_CurrentCinematicCamera->GetUp();
    Vect3f lFront = m_CurrentCinematicCamera->GetFront();

    lPos = a->GetPosition()*aPercentage + b->GetPosition()*(1 - aPercentage);
    lUp = a->GetUp()*aPercentage + b->GetUp()*(1 - aPercentage);
    lFront = a->GetLookAt()*aPercentage + b->GetLookAt()*(1 - aPercentage);

    m_CurrentCinematicCamera->SetPosition(lPos);
    m_CurrentCinematicCamera->SetUp(lUp);
    m_CurrentCinematicCamera->SetFront(lFront);
}

void CCinematicCameraPlayer::Finish()
{
    LOG_INFO_APPLICATION("Finishing camera");
    CEngine::GetInstance().SetCameraController(m_lastCameraState);
}

void CCinematicCameraPlayer::Start()
{
    LOG_INFO_APPLICATION("Starting camera");
    CEngine& lEngine = CEngine::GetInstance();
    m_lastCameraState = &(lEngine.GetCameraController());
    lEngine.SetCameraController(m_CurrentCinematicCamera);
}