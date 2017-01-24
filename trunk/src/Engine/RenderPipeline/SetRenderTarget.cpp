#include "SetRenderTarget.h"
#include "XML\XML.h"


CSetRenderTarget::CSetRenderTarget()
{

}

CSetRenderTarget::~CSetRenderTarget()
{

}

//Leerá el siguiente nodo :
/*<set_render_target name="deferred_multiple_render_target">
    <dynamic_texture name = "DiffuseMapTexture" texture_width_as_frame_buffer = "true"
	create_depth_stencil_buffer = "false" format_type = "RGBA8_UNORM" / >
	<dynamic_texture name = "LightMapTexture" texture_width_as_frame_buffer = "true"
	format_type = "RGBA8_UNORM" / >
	<dynamic_texture name = "NormalMapTexture" texture_width_as_frame_buffer = "true"
	format_type = "R8G8B8A8_UNORM" / >
	<dynamic_texture name = "DepthMapTexture" texture_width_as_frame_buffer = "true"
	format_type = "R32_FLOAT" / >
	< / set_render_target>*/

bool CSetRenderTarget::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderStagedTexture::Load(aElement);
    return lOk;
}


void CSetRenderTarget::Execute(CRenderManager& lRM)
{
    ID3D11DepthStencilView *l_DepthStencilView = m_DynamicTextures.empty() ? nullptr : m_DynamicTextures[0]->GetDepthStencilView();
    lRM.SetRenderTargets((UINT)m_DynamicTextures.size(), &m_RenderTargetViews[0],l_DepthStencilView);
}