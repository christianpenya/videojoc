#include "SetPerFrameConstantsCmd.h"
#include "XML\XML.h"
#include "Engine\Engine.h"
#include "Scenes\ConstantBufferManager.h"
#include "Lights\lightManager.h"

CSetPerFrameConstantsCmd::CSetPerFrameConstantsCmd()
{

}

CSetPerFrameConstantsCmd::~CSetPerFrameConstantsCmd()
{

}

//Esta clase realizará el set de las matrices de view y projection de la cámara activa.
//Leerá el siguiente nodo de xml: <set_per_frame_constants name="set_per_frame_cmd"/>


bool CSetPerFrameConstantsCmd::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderCmd::Load(aElement);
    return lOk;
}

void CSetPerFrameConstantsCmd::Execute(CRenderManager& lRM)
{
    lRM.SetModelMatrix(lRM.m_ModelMatrix);
    lRM.SetProjectionMatrix(lRM.m_ProjectionMatrix);

    UpdateConstants();
}

void CSetPerFrameConstantsCmd::UpdateConstants()
{
    CRenderManager& lRM = CEngine::GetInstance().GetRenderManager();
    CLightManager& l_lightManager = CEngine::GetInstance().GetLightManager();

    if (CEngine::GetInstance().ExistConstantBufferManager())
    {
        CConstantBufferManager& lConstanBufferManager = CEngine::GetInstance().GetConstantBufferManager();
        lConstanBufferManager.mFrameDesc.m_View = lRM.GetViewMatrix();
        lConstanBufferManager.mFrameDesc.m_Projection = lRM.GetProjectionMatrix();
        lConstanBufferManager.mFrameDesc.m_ViewProjection = lRM.GetViewProjectionMatrix();
        lConstanBufferManager.BindVSBuffer(lRM.GetDeviceContext(), CConstantBufferManager::CB_Frame);
    }


}