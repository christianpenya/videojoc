#include "CinematicObjectPlayer.h"
#include "CinematicObjectKey.h"
#include "Engine\Engine.h"
#include "Scenes\SceneManager.h"



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
	std::string l_NodeName = aElement->GetAttribute<std::string>("scene_node", "");
	std::string l_SceneName = aElement->GetAttribute<std::string>("scene", "");
	std::string l_LayerName = aElement->GetAttribute<std::string>("layer", "");

	
	m_SceneNode = GetSceneNode(l_SceneName, l_LayerName, l_NodeName);

    for (tinyxml2::XMLElement const *iObjKey = aElement->FirstChildElement(); iObjKey != nullptr; iObjKey = iObjKey->NextSiblingElement())
    {
        if (strcmp(iObjKey->Name(), "key")==0)
        {

            CCinematicObjectKey *lObjKey = new CCinematicObjectKey();
            lObjKey->Load(iObjKey);
            Add(Convert(lObjKey->GetTime()), lObjKey);
            if (m_CurrentKey == m_NextKey)
            {
               m_CurrentKey = lObjKey;
            }

        }


    }
	m_NextKey = m_CurrentKey;
    return lOk;



}

void CCinematicObjectPlayer::Apply(float aPercentage, CCinematicKey* A, CCinematicKey* B)
{

	Vect3f l_pos = m_SceneNode->GetPosition();
	l_pos = (A->GetPosition()*aPercentage) + (B->GetPosition()*(1 - aPercentage));
	m_SceneNode->SetPosition(l_pos);

	//TODO

}

CSceneNode* CCinematicObjectPlayer::GetSceneNode(std::string sceneName, std::string layerName, std::string nodeName){


	CSceneManager &lScenMan = CEngine::GetInstance().GetSceneManager();
	std::vector<CScene*> l_Scenes = lScenMan.GetScenes();

	for (std::vector<CScene*>::iterator it = l_Scenes.begin(); it != l_Scenes.end(); ++it){

		if ((*it)->GetName() == sceneName){
			std::vector<CLayer*> l_Layers = (*it)->GetLayers();
			for (std::vector<CLayer*>::iterator it2 = l_Layers.begin(); it2 != l_Layers.end(); ++it2){

				if ((*it2)->GetName() == layerName){

					std::vector< CSceneNode*> l_Nodes = (*it2)->GetNodes();
					for (std::vector<CSceneNode*>::iterator it3 = l_Nodes.begin(); it3 != l_Nodes.end(); ++it3){
						if ((*it3)->GetName() == nodeName){
							return (*it3);
						}

					}
				}

			}
		}

	}

}