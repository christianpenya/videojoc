#include "CinematicObjectPlayer.h"
#include "CinematicObjectKey.h"



CCinematicObjectPlayer::CCinematicObjectPlayer()
{


}

CCinematicObjectPlayer::~CCinematicObjectPlayer()
{

}

bool CCinematicObjectPlayer::Load(const CXMLElement *aElement)
{

    bool lOk = false;

    mName = aElement->GetAttribute<std::string>("name", "");
    //m_SceneNode = aElement->GetAttribute<std::string>("scene_node", "");

    for (tinyxml2::XMLElement const *iObjKey = aElement->FirstChildElement(); iObjKey != nullptr; iObjKey = iObjKey->NextSiblingElement())
    {
        if (strcmp(iObjKey->Name(), "key")==0)
        {

            CCinematicObjectKey *lObjKey = new CCinematicObjectKey();
            lOk = lOk || lObjKey->Load(iObjKey);
            Add(Convert(lObjKey->GetTime()), lObjKey);
            if (m_CurrentKey == NULL)
            {
                m_CurrentKey = lObjKey;
            }

        }


    }
    return lOk;



}

void CCinematicObjectPlayer::Apply(float aPercentage, CCinematicKey* A, CCinematicKey* B)
{



}