#include "DumbActor.h"

#include <string>
#include "XML/XML.h"
#include "Utils/Logger.h"

#include "Engine/Engine.h"
#include "Graphics/Scenes/SceneManager.h"
#include "Graphics/Scenes/Scene.h"
#include "Graphics/Scenes/Layer.h"
#include "Graphics/Scenes/SceneNode.h"

CDumbActor::CDumbActor() {}
CDumbActor::~CDumbActor() {}
void CDumbActor::Load(CXMLElement* aElement) {}

void CDumbActor::Act()
{
    //LOG_INFO_APPLICATION("ACTING! I'M ACTING!");
    /*std::string lLayerName = "triggers";
    std::string lDoorName = "trigger_pasillo";

    CSceneNode* temare = CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetLayer(lLayerName)->GetSceneNode(lDoorName);
    temare->SetActive(false);
    */
}

void CDumbActor::Update(float elapsedTime)
{
    //LOG_INFO_APPLICATION("ACTOR UPDATING");
    // is audio finished?
    m_Finished = true;

}