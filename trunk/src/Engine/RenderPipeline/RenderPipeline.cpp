#include "RenderPipeline.h"
#include "XML\XML.h"
#include "BeginRenderCmd.h"
#include "EndRenderCmd.h"
#include "SetPerFrameConstantsCmd.h"
#include "ApplyTechniquePool.h"
#include "ClearCmd.h"
#include "RenderSceneLayer.h"
#include "DrawQuad.h"
#include "RenderImGUI.h"
#include <map>
#include <functional>
#include "Engine\Engine.h"

#define RENDER_CMD_ENTRY(tag, command_class_name)  { tag, [] { return new  command_class_name();}},
std::map<std::string, std::function<CRenderCmd*(void) >> sComandsFactory =
{
    RENDER_CMD_ENTRY("begin_render", CBeginRenderCmd)
    RENDER_CMD_ENTRY("end_render", CEndRenderCmd)
    RENDER_CMD_ENTRY("set_per_frame_constants", CSetPerFrameConstantsCmd)
    RENDER_CMD_ENTRY("apply_technique_pool", CApplyTechniquePool)
    RENDER_CMD_ENTRY("render_layer", CRenderSceneLayer)
    RENDER_CMD_ENTRY("draw_quad", CDrawQuad)
    RENDER_CMD_ENTRY("render_imgui", CRenderImGUI)
    RENDER_CMD_ENTRY("clear", CClearCmd)
};


/*<render_pipeline>
<begin_render name = "begin_render_main_loop" / >
<clear name = "clear_cmd" render_target = "false" depth_stencil = "true" color = "0.25 0.25 0.25 0" />
<set_per_frame_constants name = "set_per_frame_cmd" / >
<apply_technique_pool name = "apply_forward_technique" pool_name = "forward" / >
<render_layer name = "render_opaque" layer = "opaque" / >
<!--<draw_quad name = "full_screen_quad" viewport_position = "0 0" viewport_size = "800 600" pixel_shader = "DefaultQuadPS" / >-->
<render_imgui name = "imediate_gui_render" / >
<end_render name = "end_render_main_loop" / >
< / render_pipeline>*/

CRenderPipeline::CRenderPipeline()
{

}

CRenderPipeline::~CRenderPipeline()
{

}

bool CRenderPipeline::Load(const std::string& aFilename)
{
    bool lOk = false;

    CXMLDocument document;
    EXMLParseError error = document.LoadFile((aFilename).c_str());

    if (base::xml::SucceedLoad(error))
    {
        m_Filename = aFilename;
        CXMLElement * lRenderPipeline = document.FirstChildElement("render_pipeline");

        if (lRenderPipeline)
        {
            for (tinyxml2::XMLElement *iRenderPipeline = lRenderPipeline->FirstChildElement(); iRenderPipeline != nullptr; iRenderPipeline = iRenderPipeline->NextSiblingElement())
            {
                CRenderCmd* lCommand = sComandsFactory[iRenderPipeline->Name()]();
                if (lCommand->Load(iRenderPipeline))
                {
                    Add(lCommand->GetName(), lCommand);
                }
            }
            lOk = true;
        }
    }

    return lOk;
}

void CRenderPipeline::Execute()
{
    CRenderManager& lRenderManager = CEngine::GetInstance().GetRenderManager();
    /*    for (size_t i = 0; i < m_ResourcesVector.size(); ++i)
        {
            m_ResourcesVector[i]->Execute(lRenderManager);
        }*/
}


void CRenderPipeline::Reload()
{
    Destroy();
    Load(m_Filename);
}


