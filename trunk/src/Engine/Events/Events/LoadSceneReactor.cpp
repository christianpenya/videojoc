#include "LoadSceneReactor.h"

#include <string>
#include "XML/XML.h"
#include "Utils/Logger.h"

#include "Engine/Engine.h"
#include "Graphics/Scenes/SceneManager.h"
#include "Graphics/Materials/MaterialManager.h"

#include "Utils/CheckedRelease.h"

CLoadSceneReactor::CLoadSceneReactor() {}

CLoadSceneReactor::~CLoadSceneReactor()
{
}

void CLoadSceneReactor::Load(CXMLElement* aElement)
{
    m_Scene = aElement->GetAttribute<std::string>("scene", "");
    assert(m_Scene != "");
}

void CLoadSceneReactor::React()
{
    LOG_INFO_APPLICATION("React reacting!");

    CEngine::GetInstance().GetMaterialManager().Load(m_Scene, true);
    CEngine::GetInstance().GetSceneManager().LoadNewScene(m_Scene);
}

void CLoadSceneReactor::Update(float elapsedTime)
{
    LOG_INFO_APPLICATION("React updating!");
}
