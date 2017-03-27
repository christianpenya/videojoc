#include "SetPerFrameConstantsCmd.h"
#include "XML/XML.h"
#include "Engine/Engine.h"
#include "Graphics/Buffers/ConstantBufferManager.h"
#include "Graphics/Lights/lightManager.h"
#include <ctime>

CSetPerFrameConstantsCmd::CSetPerFrameConstantsCmd() {}
CSetPerFrameConstantsCmd::~CSetPerFrameConstantsCmd() {}

//Esta clase realizar� el set de las matrices de view y projection de la c�mara activa.
//Leer� el siguiente nodo de xml: <set_per_frame_constants name="set_per_frame_cmd"/>

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
    CCameraController& lCC = CEngine::GetInstance().GetCameraController();
    CLightManager& l_lightManager = CEngine::GetInstance().GetLightManager();

    if (CEngine::GetInstance().ExistConstantBufferManager())
    {
        CConstantBufferManager& lConstanBufferManager = CEngine::GetInstance().GetConstantBufferManager();
        lConstanBufferManager.mFrameDesc.m_View = lRM.GetViewMatrix();
        lConstanBufferManager.mFrameDesc.m_Projection = lRM.GetProjectionMatrix();
        lConstanBufferManager.mFrameDesc.m_ViewProjection = lRM.GetViewProjectionMatrix();
        lConstanBufferManager.mFrameDesc.m_CameraPosition = lCC.getPosition();
        lConstanBufferManager.mFrameDesc.m_CameraFrontVector = lCC.getFront();
        lConstanBufferManager.mFrameDesc.m_CameraUpVector = lCC.getUp();
        lConstanBufferManager.mFrameDesc.m_TimeSeconds.x = std::clock();
        lConstanBufferManager.BindBuffer(lRM.GetDeviceContext(), CConstantBufferManager::CB_Frame);
    }
}
