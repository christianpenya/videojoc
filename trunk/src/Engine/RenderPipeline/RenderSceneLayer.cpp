#include "RenderSceneLayer.h"
#include "XML\XML.h"
#include "Scenes\SceneManager.h"
#include "Engine\engine.h"

CRenderSceneLayer::CRenderSceneLayer()
{

}

CRenderSceneLayer::~CRenderSceneLayer()
{

}

//Leerá el siguiente nodo :
//<render_layer layer="opaque"/>

bool CRenderSceneLayer::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderCmd::Load(aElement);
    if (lOk)
        m_LayerName = aElement->GetAttribute<std::string>("layer", "opaque");

    return lOk;
}

void CRenderSceneLayer::Execute(CRenderManager& lRM)
{
    if (CEngine::GetInstance().ExistSceneManager())
    {
        CSceneManager& lSceneManager = CEngine::GetInstance().GetSceneManager();
        lSceneManager.Render(m_LayerName);
    }
}