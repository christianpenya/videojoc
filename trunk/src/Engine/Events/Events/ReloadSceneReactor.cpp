#include "ReloadSceneReactor.h"

#include <string>
#include "XML/XML.h"
#include "Utils/Logger.h"

#include "Engine/Engine.h"
#include "Graphics/Scenes/SceneManager.h"
#include "Graphics/Scenes/Scene.h"
#include "Graphics/Scenes/Layer.h"
#include "Graphics/Scenes/SceneNode.h"
#include "Graphics/Scenes/SceneMesh.h"
#include "Physx/PhysxManager.h"

#include "Utils/CheckedRelease.h"

CReloadSceneReactor::CReloadSceneReactor() {}

CReloadSceneReactor::~CReloadSceneReactor()
{
}

void CReloadSceneReactor::Load(CXMLElement* aElement)
{
    LOG_INFO_APPLICATION("Yo what up!");
}

void CReloadSceneReactor::React()
{
    LOG_INFO_APPLICATION("React reloading current scene!");
    CEngine::GetInstance().GetSceneManager().ReloadCurrentScene();
    m_Finished = true;
}

void CReloadSceneReactor::Update(float elapsedTime)
{
    LOG_INFO_APPLICATION("React reloading current scene, updating!");
}
