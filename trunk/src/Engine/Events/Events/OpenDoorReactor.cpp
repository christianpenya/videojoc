#include "OpenDoorReactor.h"

#include <string>
#include "XML/XML.h"
#include "Utils/Logger.h"

#include "Engine/Engine.h"
#include "Graphics/Scenes/SceneManager.h"
#include "Graphics/Scenes/Scene.h"
#include "Graphics/Scenes/Layer.h"
#include "Graphics/Scenes/SceneNode.h"
#include "Physx/PhysxManager.h"

COpenDoorReactor::COpenDoorReactor() {}

COpenDoorReactor::~COpenDoorReactor() {}

void COpenDoorReactor::Load(CXMLElement* aElement)
{
    std::string lLayerName = aElement->GetAttribute<std::string>("layer", "");
    std::string lObjectName = aElement->GetAttribute<std::string>("obj_name", "");

    assert(lLayerName != "");
    assert(lObjectName != "");

    mDoor = CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetLayer(lLayerName)->GetSceneNode(lObjectName);
}

void COpenDoorReactor::React()
{
    // std::string lLayerName = "opaque";
    // std::string lDoorName = "puerta666";

    // CSceneNode* lDoor = CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetLayer(lLayerName)->GetSceneNode(lDoorName);
    // LOG_INFO_APPLICATION(lDoor->GetName().c_str());
    //mDoor->Deactivate();
    LOG_INFO_APPLICATION("Hold the DOOOOOOOR! HOLD THE DOOOOOR!");
}

void COpenDoorReactor::Update(float elapsedTime)
{
    LOG_INFO_APPLICATION("React updating!");

    if (!mDoor)
    {
        LOG_INFO_APPLICATION("Door is missing in scene!");
        return;
    }

    if (mDoor->GetActive())
    {
        //mDoor->Deactivate();
        mDoor->SetYaw(mDoor->GetYaw() + elapsedTime);
        CEngine::GetInstance().GetPhysXManager().SetActorTransform(mDoor->GetName(), mDoor->GetPosition(), mDoor->GetYaw() + elapsedTime, mDoor->GetPitch(), mDoor->GetRoll());
    }
}
