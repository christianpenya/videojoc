#include "DrawQuad.h"
#include "XML/XML.h"
#include "Utils/StringUtils.h"
#include "Graphics/Effects/PixelShader.h"
#include "Render/RenderManager.h"
#include "Engine/engine.h"

CDrawQuad::CDrawQuad(): mQuad(nullptr), mMaterial(nullptr) {}

CDrawQuad::~CDrawQuad() {}

//Leera el nodo
//<draw_quad material="DrawQuadMaterial" viewport_size="128 128" viewport_position="128 0">

bool CDrawQuad::Load(const CXMLElement * aElement)
{
    bool lOk = CRenderStagedTexture::Load(aElement);
    if (lOk)
    {
        m_ViewportSize = aElement->GetAttribute<Vect2u>("viewport_size", Vect2u(128, 128));
        m_ViewportPosition = aElement->GetAttribute<Vect2u>("viewport_position", Vect2u(128, 0));
        CMaterialManager& lMaterialManager = CEngine::GetInstance().GetMaterialManager();
        mMaterial = lMaterialManager(aElement->GetAttribute<std::string>("material", ""));

        mQuad = new CQuad();
        mQuad->Init();
    }

    return lOk;
}

void CDrawQuad::Execute(CRenderManager& lRM)
{
    lRM.SetViewport(m_ViewportPosition, m_ViewportSize);
    ActivateTextures();
    mMaterial->Apply();

    mQuad->Render();
    lRM.ResetViewport();
}
