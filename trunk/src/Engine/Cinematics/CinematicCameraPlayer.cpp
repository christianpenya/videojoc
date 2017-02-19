#include "CinematicCameraPlayer.h"
#include "CinematicCameraKey.h"




CCinematicCameraPlayer::CCinematicCameraPlayer()
{


}

CCinematicCameraPlayer::~CCinematicCameraPlayer()
{

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
			if (m_CurrentKey == m_NextKey)
			{
				m_CurrentKey = lCamKey;
			}
        }


	}	
	m_NextKey = m_CurrentKey;
	return lOk;


}

void CCinematicCameraPlayer::Apply(float aPercentage, CCinematicKey* A, CCinematicKey* B)
{
	CCinematicCameraKey* a = dynamic_cast<CCinematicCameraKey*>(A);
	CCinematicCameraKey* b = dynamic_cast<CCinematicCameraKey*>(B);
	
	CCameraController *cam = &(CEngine::GetInstance().GetCameraController());
	Vect3f lPos = cam->getPosition();
	Vect3f lUp = cam->getUp();
	Vect3f lFront = cam->getFront();
	lPos = a->GetPosition()*aPercentage + b->GetPosition()*(1 - aPercentage);
	lUp = a->GetUp()*aPercentage + b->GetUp()*(1 - aPercentage);
	lFront = a->GetLookAt()*aPercentage + b->GetLookAt()*(1 - aPercentage);
	
	
	cam->setPosition(lPos);
	cam->setUp(lUp);
	cam->setFront(lFront);





}