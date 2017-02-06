#include "RenderStagedTexture.h"
#include "XML\XML.h"
#include "Engine\engine.h"

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
        if (strcmp(iNTexture->Name(), "dynamic_texture") == 0)
        {
//            CDynamicTexture* l_DynamicTexture = new CDynamicTexture(iNTexture);
//            m_DynamicTextures.push_back(l_DynamicTexture);
        }
        else if (strcmp(iNTexture->Name(), "texture") == 0)
        {
            //CTexture* l_Texture = new CTexture(iNTexture->GetAttribute<std::string>("file", "DiffuseMapTexture"));
            //CStagedTexture* l_stagedTexture = new CStagedTexture(iNTexture->GetAttribute<uint32>("stage_id", 1), l_Texture);
            //l_stagedTexture->m_filename = iNTexture->GetAttribute<std::string>("file", "DiffuseMapTexture");
            //m_StagedTextures.push_back(l_stagedTexture);
        }
    }

    return lOk;

}

/*void CRenderStagedTexture::CreateRenderTargetViewVector()
{
    CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();
    lRenderManager.SetRendertarget();
}*/

void CRenderStagedTexture::ActivateTextures()
{

}

CRenderStagedTexture::CStagedTexture::CStagedTexture(uint32 aStageId, CTexture * aTexture)
{
    m_StageId = aStageId;
    m_Texture = aTexture;
}

void CRenderStagedTexture::CreateRenderTargetViewVector(CRenderManager& lRM)
{
    lRM.SetRendertarget();
}

void CRenderStagedTexture::Execute(CRenderManager& lRM)
{
    CreateRenderTargetViewVector(lRM);
}
