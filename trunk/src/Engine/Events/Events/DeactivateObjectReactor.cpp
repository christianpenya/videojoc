#include "XML/XML.h"

#include "Engine/Engine.h"
#include "Graphics/Scenes/SceneManager.h"
#include "Graphics/Scenes/Scene.h"
#include "Graphics/Scenes/Layer.h"
#include "Graphics/Scenes/SceneNode.h"
#include "Graphics/Scenes/SceneMesh.h"

#include "Utils/CheckedRelease.h"
#include "DeactivateObjectReactor.h"

CDeactivateObjReactor::CDeactivateObjReactor() {}

CDeactivateObjReactor::~CDeactivateObjReactor()
{
    __H_CHECKED_RELEASE__(mObj);
}

void CDeactivateObjReactor::Load(CXMLElement* aElement)
{
    std::string lLayerName = aElement->GetAttribute<std::string>("layer", "");
    std::string lObjectName = aElement->GetAttribute<std::string>("obj_name", "");

    assert(lLayerName != "");
    assert(lObjectName != "");

    CLayer* lLayer = CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetLayer(lLayerName);
    if (lLayer && lLayer->GetActive())
    {
        mObj = CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetLayer(lLayerName)->GetSceneNode(lObjectName);
    }
    else
    {
        mObj = false;
    }
}

void CDeactivateObjReactor::React()
{
    //LOG_INFO_APPLICATION("Hold the DOOOOOOOR! HOLD THE DOOOOOR!");
    if (mObj)
    {
        ((CSceneMesh*)mObj)->Deactivate();
    }

}

void CDeactivateObjReactor::Update(float elapsedTime)
{
    m_Finished = true;
}
