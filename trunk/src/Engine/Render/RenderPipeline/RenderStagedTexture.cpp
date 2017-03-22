#include "RenderStagedTexture.h"
#include "XML\XML.h"
#include "Engine\engine.h"
#include "Graphics\Textures\TextureManager.h"
#include "Graphics\Textures\Texture.h"

CRenderStagedTexture::CRenderStagedTexture() :
    m_ViewportSize(0),
    m_ViewportPosition(0)
    //TODO terminar de inicializar
{}
CRenderStagedTexture::~CRenderStagedTexture() {}

//Leera el nodo
//<dynamic_texture name="DiffuseMapTexture" texture_width_as_frame_buffer="true" create_depth_stencil_buffer="false" format_type="RGBA8_UNORM"/>
//<texture stage_id = "0" file = "DiffuseMapTexture" / >

bool CRenderStagedTexture::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderCmd::Load(aElement);

    CMaterialManager& lMaterialManager = CEngine::GetInstance().GetMaterialManager();
    CTextureManager& lTextureManager = CEngine::GetInstance().GetTextureManager();

    // TODO este atributo parece no existir en el fichero, material es siempre null
    CMaterial* lMaterial = lMaterialManager(aElement->GetAttribute<std::string>("material", ""));

    for (tinyxml2::XMLElement const *iNTexture = aElement->FirstChildElement(); iNTexture != nullptr; iNTexture = iNTexture->NextSiblingElement())
    {
        if (strcmp(iNTexture->Name(), "dynamic_texture") == 0)
        {
            CDynamicTexture* l_DynamicTexture = new CDynamicTexture(iNTexture);
            lTextureManager.AddTexture(*l_DynamicTexture);
            AddDynamicTextureMaterial(l_DynamicTexture, lMaterial);
            m_RenderTargetViews.push_back(l_DynamicTexture->GetRenderTargetView());
        }
        else if (strcmp(iNTexture->Name(), "texture") == 0)
        {
            CTexture* l_Texture = lTextureManager.GetTexture(iNTexture->GetAttribute<std::string>("name", "AlbedoTexture"));
            CStagedTexture* l_stagedTexture = new CStagedTexture(iNTexture->GetAttribute<uint32>("stage_id", 1), l_Texture);
            l_stagedTexture->m_filename = iNTexture->GetAttribute<std::string>("name", "AlbedoTexture");
            m_StagedTextures.push_back(l_stagedTexture);
        }
    }

    CreateRenderTargetViewVector();

    return lOk;

}

void CRenderStagedTexture::CreateRenderTargetViewVector()
{
    CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();
    lRenderManager.SetRendertarget();
}

void CRenderStagedTexture::ActivateTextures()
{
    for (size_t i = 0; i < m_StagedTextures.size(); ++i)
        m_StagedTextures[i]->Activate();
}

void CRenderStagedTexture::AddDynamicTextureMaterial(CDynamicTexture* aDynamicTexture, CMaterial* aMaterial)
{
    CDynamicTextureMaterial* l_DynamicTextureMaterial = new CDynamicTextureMaterial(aDynamicTexture, aMaterial);
    m_DynamicTexturesMaterials.push_back(l_DynamicTextureMaterial);
}

CRenderStagedTexture::CStagedTexture::CStagedTexture(uint32 aStageId, CTexture * aTexture)
{
    m_StageId = aStageId;
    m_Texture = aTexture;
}

void CRenderStagedTexture::CStagedTexture::Activate()
{
    CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();
    if (m_Texture!=nullptr)
        m_Texture->Bind(m_StageId, lRenderManager.GetDeviceContext());
}

void CRenderStagedTexture::Execute(CRenderManager& lRM)
{
    lRM.SetViewport(m_ViewportPosition, m_ViewportSize);
}
