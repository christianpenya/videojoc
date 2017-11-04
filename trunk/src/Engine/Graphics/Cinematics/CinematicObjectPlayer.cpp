#include "CinematicObjectPlayer.h"
#include "CinematicObjectKey.h"
#include "Engine\Engine.h"
#include "Graphics/Scenes/SceneManager.h"
#include "XML\tinyxml2\tinyxml2.h"
#include "Utils/CheckedDelete.h"
#include "Utils/CheckedRelease.h"

CCinematicObjectPlayer::CCinematicObjectPlayer():
    m_SceneNode(nullptr) {}

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

        }
    }


    return lOk && (m_SceneNode != nullptr);
}

void CCinematicObjectPlayer::Apply(float aPercentage, CCinematicKey* A, CCinematicKey* B)
{
    CCinematicObjectKey* a = dynamic_cast<CCinematicObjectKey*>(A);
    CCinematicObjectKey* b = dynamic_cast<CCinematicObjectKey*>(B);

    Vect3f l_pos = m_SceneNode->GetPosition();
    float l_yaw = m_SceneNode->GetYaw();
    float l_pitch = m_SceneNode->GetPitch();
    float l_roll = m_SceneNode->GetPitch();
    Vect3f l_scale = m_SceneNode->GetScale();

    l_pos = (a->GetPosition()*aPercentage) + (b->GetPosition()*(1 - aPercentage));
    l_yaw = (a->GetRotation().x*aPercentage) + (b->GetRotation().x*(1 - aPercentage));
    l_pitch = (a->GetRotation().y*aPercentage) + (b->GetRotation().y*(1 - aPercentage));
    l_roll = (a->GetRotation().z*aPercentage) + (b->GetRotation().z*(1 - aPercentage));
    l_scale = (a->GetScale()*aPercentage) + (b->GetScale()*(1 - aPercentage));

    m_SceneNode->SetPosition(l_pos);
    m_SceneNode->SetYaw(l_yaw);
    m_SceneNode->SetPitch(l_pitch);
    m_SceneNode->SetRoll(l_roll);
    //m_SceneNode->SetScale(l_scale);
}

CSceneNode* CCinematicObjectPlayer::GetSceneNode(std::string sceneName, std::string layerName, std::string nodeName)
{
    CSceneNode * lOut = nullptr;

    CSceneManager* lScenMan = &CEngine::GetInstance().GetSceneManager();
    CScene* scene = lScenMan->GetCurrentScene();
    CLayer* layer = scene->GetLayer(layerName);
    lOut = layer->GetSceneNode(nodeName);
    return lOut;


    /*
    std::vector<CScene*> l_Scenes = lScenMan.GetScenes();

    for (std::vector<CScene*>::iterator it = l_Scenes.begin(); it != l_Scenes.end(); ++it)
    {
        if ((*it)->GetName() == sceneName)
        {
            std::vector<CLayer*> l_Layers = (*it)->GetLayers();
            for (std::vector<CLayer*>::iterator it2 = l_Layers.begin(); it2 != l_Layers.end(); ++it2)
            {
                if ((*it2)->GetName() == layerName)
                {
                    std::vector< CSceneNode*> l_Nodes = (*it2)->GetNodes();
                    for (std::vector<CSceneNode*>::iterator it3 = l_Nodes.begin(); it3 != l_Nodes.end(); ++it3)
                    {
                        if ((*it3)->GetName() == nodeName)
                        {
                            lOut = (*it3);
                        }
                    }
                }
            }
        }
    }

    return lOut;
    */
}
