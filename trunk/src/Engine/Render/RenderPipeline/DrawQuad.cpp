#include "DrawQuad.h"
#include "XML\XML.h"
#include "Utils\StringUtils.h"
#include "Graphics/Effects/PixelShader.h"
#include "Render/RenderManager.h"

CDrawQuad::CDrawQuad() {}
CDrawQuad::~CDrawQuad() {}

//Leera el nodo
//<draw_quad material="DrawQuadMaterial" viewport_size="128 128" viewport_position="128 0">

bool CDrawQuad::Load(const CXMLElement * aElement)
{
    bool lOk = CRenderStagedTexture::Load(aElement);
    if (lOk)
    {
        Vect2u m_ViewportSize = aElement->GetAttribute<Vect2u>("viewport_size", Vect2u(128, 128));
        Vect2u m_ViewportPosition = aElement->GetAttribute<Vect2u>("viewport_position", Vect2u(128, 0));

        tinyxml2::XMLElement *iMaterial = aElement->GetAttribute<std::string>("material","");
        mMaterial = new CMaterial(iMaterial);

        mQuad = new CQuad();
        mQuad->Render();

    }
    return lOk;

}

void CDrawQuad::Execute(CRenderManager& lRM)
{
    lRM.SetViewport(m_ViewportPosition, m_ViewportSize);
    mQuad->Render();
    lRM.ResetViewport();
}
