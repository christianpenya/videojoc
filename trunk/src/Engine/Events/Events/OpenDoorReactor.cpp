#include "OpenDoorReactor.h"

#include <string>
#include "XML/XML.h"
#include "Utils/Logger.h"

#include "Engine/Engine.h"
#include "Graphics/Scenes/SceneManager.h"
#include "Graphics/Scenes/Scene.h"
#include "Graphics/Scenes/Layer.h"
#include "Graphics/Scenes/SceneNode.h"

COpenDoorReactor::COpenDoorReactor() {}
COpenDoorReactor::~COpenDoorReactor() {}

void COpenDoorReactor::React()
{
    std::string lLayerName = "opaque";
    std::string lDoorName = "puerta666";

    CSceneNode* temare = CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetLayer(lLayerName)->GetSceneNode(lDoorName);
    LOG_INFO_APPLICATION(temare->GetName().c_str());
    temare->Deactivate();
    LOG_INFO_APPLICATION("Hold the DOOOOOOOR! HOLD THE DOOOOOR!");
}

void COpenDoorReactor::Update()
{
    LOG_INFO_APPLICATION("React updating!");
}