#include "RenderStagedTexture.h"
#include "XML\XML.h"


CRenderStagedTexture::CRenderStagedTexture()
{

}

CRenderStagedTexture::~CRenderStagedTexture()
{

}


//Leera el nodo
//<dynamic_texture name="DiffuseMapTexture" texture_width_as_frame_buffer="true" create_depth_stencil_buffer="false" format_type="RGBA8_UNORM"/>

//<texture stage_id = "0" file = "DiffuseMapTexture" / >

bool CRenderStagedTexture::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderCmd::Load(aElement);

    for (tinyxml2::XMLElement const *iNTexture = aElement->FirstChildElement(); iNTexture != nullptr; iNTexture = iNTexture->NextSiblingElement())
    {
        if (strcmp(aElement->Name(), "dynamic_texture") == 0)
        {

        }
    }
    CreateRenderTargetViewVector();
    return lOk;

}

void CRenderStagedTexture::CreateRenderTargetViewVector()
{

}

void CRenderStagedTexture::ActivateTextures()
{

}






CRenderStagedTexture::CStagedTexture::CStagedTexture(uint32 aStageId, CTexture * aTexture)
{
    m_StageId = aStageId;
    m_Texture = aTexture;
}

